/* $Id: geese_pick.cc,v 1.1 2010-09-09 20:38:58 grahn Exp $
 *
 * Copyright (c) 2010 Jörgen Grahn
 * All rights reserved.
 *
 */
#include "regex.h"

#include <iostream>
#include <fstream>
#include <map>
#include <vector>
#include <string>

#include <cstdlib>
#include <cerrno>
#include <cstring>
#include <getopt.h>

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

namespace {

    using std::vector;
    using std::string;

    struct Map {};

    typedef std::map<std::string, Map> Library;
    typedef std::vector<std::string> Lines;

    std::string basename(const std::string& path)
    {
	string::size_type n = path.rfind('/');
	if(n==string::npos) {
	    return path;
	}
	return string(path, n+1);
    }

    bool is_md5(const std::string&s)
    {
	return s.size()==32 && 
	    s.find_first_not_of("0123456789abcdef")==string::npos;
    }

    void parse(Library& lib,
	       const Lines& acc,
	       std::ostream& log)
    {
	vector<string> names;
	Map map;

	for(Lines::const_iterator i = acc.begin(); i!=acc.end(); ++i) {
	    const string& s = *i;
	    
	}

	for(vector<string>::const_iterator i = names.begin(); i!=names.end(); ++i) {
	    lib[*i] = map;
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
}


int main(int argc, char ** argv)
{
    using std::string;

    const string prog = argv[0];
    const string usage = string("usage: ")
	+ prog
	+ " -f mapping-file map-file";
    const char optstring[] = "+f:h";
    struct option long_options[] = {
	{"version", 0, 0, 'v'},
	{"help", 0, 0, 'h'},
	{0, 0, 0, 0}
    };

    string libfile;

    std::cin.sync_with_stdio(false);
    std::cout.sync_with_stdio(false);

    int ch;
    while((ch = getopt_long(argc, argv,
			    optstring, &long_options[0], 0)) != -1) {
	switch(ch) {
	case 'f':
	    libfile = optarg;
	    break;
	case 'h':
	    std::cout << usage << '\n';
	    return 0;
	case 'v':
	    std::cout << prog << "\n$Name:  $\n"
		      << "Copyright (c) 2004 - 2010 Jörgen Grahn\n";
	    return 0;
	    break;
	case ':':
	case '?':
	    std::cerr << usage << '\n';
	    return 1;
	    break;
	default:
	    break;
	}
    }

    if(libfile.empty() || optind==argc) {
	std::cerr << usage << '\n';
	return 1;
    }

    const string mapfile = argv[optind];

    const Library library = parse_lib(libfile, std::cerr);
    if(library.empty()) {
	return 1;
    }

    return 0;
}
