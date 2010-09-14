/* $Id: library.cc,v 1.9 2010-09-14 18:56:06 grahn Exp $
 *
 * Copyright (c) 2010 J�rgen Grahn
 * All rights reserved.
 *
 */
#include "library.h"
#include "regex.h"
#include "worldfile.h"
#include "globbing.h"

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

	if(s.empty()) {
	    if(!acc.empty()) {
		parse(lib, acc, libfile, n, log);
		acc.clear();
	    }
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


namespace {

    std::string basename(const std::string& path)
    {
	std::string::size_type n = path.rfind('/');
	if(n==std::string::npos) {
	    return path;
	}
	return std::string(path, n+1);
    }

    /**
     * "foo" -> "�/foo"
     * "foo/bar" -> "foo/�/bar"
     */
    std::string star(const std::string& path)
    {
	string p = path;
	string::size_type n = p.rfind('/');
	if(n==string::npos) {
	    return p.insert(0, 1, '*');
	}
	return p.insert(n, "/*");
    }

    std::string barw(const std::string& path)
    {
	return path + "w";
    }

    std::string brw(const std::string& path)
    {
	string p = path;
	string::size_type n = p.rfind('.');
	if(n==string::npos) return "";
	if(n+4!=p.size()) return "";
	return p.erase(n+2, 1) + "w";
    }

    std::string tfw(const std::string& path)
    {
	string p = path;
	string::size_type n = p.rfind('.');
	if(n==string::npos) return "";
	return p.replace(n, string::npos, ".tfw");
    }
}


/**
 * Find the mapping for image file 'mapfile', if one exists.  Mostly
 * the transform, but also MD5sum and dimensions (if found in
 * 'libfile'). Also log I/O errors and warnings to 'log'.
 *
 * The search order is (let's pretend 'mapfile' is 'mapfile.bar'):
 * - only 'worldfile' is searched if it's given
 * - 'libfile', a geese mapping file
 * - a world file mapfile.barw
 * - a world file mapfile.brw
 * - a world file mapfile.twf (because the GIS community has a TIFF fetish)
 *
 */
Map find_mapping(const string& mapfile,
		 const string& libfile,
		 const string& worldfile,
		 std::ostream& log)
{
    Map mapping;

    /* XXX lame error handling */

    if(!worldfile.empty()) {
	mapping.empty = !parse_world(mapping.t, worldfile, log);
	return mapping;
    }

    if(!libfile.empty()) {
	const Library library = parse_lib(libfile, log);

	const Library::const_iterator i = library.find(basename(mapfile));
	if(i!=library.end()) {
	    mapping = i->second;
	    return mapping;
	}
    }

    vector<string> wnames;
    wnames.push_back(barw(mapfile));
    glob(star(barw(mapfile)), wnames);
    const string brw = ::brw(mapfile);
    if(!brw.empty()) {
	wnames.push_back(brw);
	glob(star(brw), wnames);
    }
    const string tfw = ::tfw(mapfile);
    if(!tfw.empty()) {
	wnames.push_back(tfw);
	glob(star(tfw), wnames);
    }

    for(vector<string>::const_iterator i=wnames.begin(); i!=wnames.end(); ++i) {

	mapping.empty = !parse_world(mapping.t, *i, log);
	if(!mapping.empty) break;
    }

    return mapping;
}
