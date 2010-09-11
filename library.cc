/* $Id: library.cc,v 1.2 2010-09-11 15:17:04 grahn Exp $
 *
 * Copyright (c) 2010 Jörgen Grahn
 * All rights reserved.
 *
 */
#include "library.h"
#include "regex.h"
#include "transform.h"

#include <map>
#include <vector>

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

    std::string basename(const std::string& path)
    {
	string::size_type n = path.rfind('/');
	if(n==string::npos) {
	    return path;
	}
	return string(path, n+1);
    }

    void parse(Library& lib,
	       const Lines& acc,
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

	Transform T;
	const Map map(checksums, Dimensions(), T);
	for(vector<string>::const_iterator i = names.begin(); i!=names.end(); ++i) {
	    lib[*i] = map;
	}
    }

}


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

    while(std::getline(is, s)) {
	if(!s.empty() && s[0] == '#') {
	    continue;
	}

	if(s.empty() && !acc.empty()) {
	    parse(lib, acc, log);
	    acc.clear();
	    continue;
	}

	acc.push_back(s);
    }

    if(is.fail()) {
	log << "error reading " << libfile << ": "
	    << std::strerror(errno) << '\n';
    }
    return lib;
}
