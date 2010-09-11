/* $Id: library.cc,v 1.6 2010-09-11 18:18:58 grahn Exp $
 *
 * Copyright (c) 2010 Jörgen Grahn
 * All rights reserved.
 *
 */
#include "library.h"
#include "regex.h"

#include <cstdlib>
#include <cctype>
#include <iostream>
#include <fstream>
#include <cerrno>
#include <cstring>


using std::vector;
using std::string;

typedef std::vector<std::string> Lines;

namespace {

    namespace re {

	/* 21da8fb51edd4c398765e012b9cc8738 */
	Regex md5("^[[:xdigit:]]{32}$");

	/* 2146 x 2578 */
	Regex dimension("^ *[0-9]+ *x *[0-9]+$");

	/* 6447181 1356800 -> 0,0 */
	Regex mapping("^ *"
		      "[0-9]+(\\.[0-9]+)?"
		      " +"
		      "[0-9]+(\\.[0-9]+)?"
		      " *-> *"
		      "[0-9]+(\\.[0-9]+)?"
		      " *, *"
		      "[0-9]+(\\.[0-9]+)?");
    }

    struct Mapping {
	double a;
	double b;
	double c;
	double d;
    };

    void parse(Library& lib,
	       const Lines& acc,
	       const std::string& filename,
	       const unsigned lineno,
	       std::ostream& log)
    {
	vector<string> names;
	vector<string> checksums;

	Lines::const_iterator i;
	for(i = acc.begin(); i!=acc.end(); ++i) {
	    const string& s = *i;
	    if(re::dimension.match(s) || re::mapping.match(s)) break;
	    if(re::md5.match(s)) checksums.push_back(s);
	    names.push_back(s);
	}

	Dimensions dim;
	vector<Mapping> mappings;

	for(; i!=acc.end(); ++i) {
	    const std::string& s = *i;
	    const char* p = s.c_str();
	    if(re::dimension.match(s)) {
		char* end;
		dim.width = std::strtod(p, &end);
		assert(end!=p);
		assert(*end);
		p = end;
		while(isspace(*p)) p++;
		assert(*p=='x');
		p++;
		dim.height = std::strtod(p, &end);
		assert(end!=p);
	    }
	    else if(re::mapping.match(s)) {
		Mapping m;
		char* end;
		m.a = std::strtod(p, &end);
		p = end;
		m.b = std::strtod(p, &end);
		p = end;
		while(isspace(*p)) p++;
		assert(*p=='-');
		p++;
		assert(*p=='>');
		p++;
		m.c = std::strtod(p, &end);
		p = end;
		while(isspace(*p)) p++;
		assert(*p==',');
		p++;
		m.d = std::strtod(p, &end);

		mappings.push_back(m);
	    }
	    else {
		log << filename << ':' << lineno << ": "
		    << "warning: ignoring line \"" << s << "\"\n";
	    }
	}

	if(names.empty()) {
	    log << filename << ':' << lineno << ": "
		<< "warning: entry without file name discarded\n";
	    return;
	}
	if(mappings.size()<2) {
	    log << filename << ':' << lineno << ": "
		<< "warning: entry without mappings discarded\n";
	    return;
	}
	if(mappings.size()>2) {
	    log << filename << ':' << lineno << ": "
		<< "warning: ignoring extra mappings (I only use two)\n";
	}

	const Mapping& a = mappings[0];
	const Mapping& b = mappings[1];

	const Map map(checksums, dim,
		      Transform(RT90(a.a, a.b), Pixel(a.c, a.d),
				RT90(b.a, b.b), Pixel(b.c, b.d)));

	for(vector<string>::const_iterator i = names.begin(); i!=names.end(); ++i) {
	    lib[*i] = map;
	}
    }

}


/**
 * Read a map "library" from 'libfile', printing errors/warnings to 'log'.
 * Returns an empty library in case of complete failure.
 */
Library parse_lib(const std::string& libfile, std::ostream& log)
{
    Library lib;
    std::ifstream is(libfile.c_str());
    if(is.fail()) {
	log << "error: cannot open " << libfile << ": "
	    << std::strerror(errno) << '\n';
	return lib;
    }

    Lines acc;
    std::string s;
    unsigned n = 0;

    while(std::getline(is, s)) {
	++n;
	if(!s.empty() && s[0] == '#') {
	    continue;
	}

	if(s.empty() && !acc.empty()) {
	    parse(lib, acc, libfile, n, log);
	    acc.clear();
	    continue;
	}

	acc.push_back(s);
    }

    if(!is.eof()) {
	log << "error reading " << libfile << ": "
	    << std::strerror(errno) << '\n';
    }
    return lib;
}
