/* $Id: geese_pick.cc,v 1.3 2010-09-11 08:39:55 grahn Exp $
 *
 * Copyright (c) 2010 J�rgen Grahn
 * All rights reserved.
 *
 */
#include <iostream>
#include <fstream>
#include <string>

#include <cstdlib>
#include <cerrno>
#include <cstring>
#include <getopt.h>

#include "library.h"


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
		      << "Copyright (c) 2004 - 2010 J�rgen Grahn\n";
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
