/* $Id: geese_world.cc,v 1.1 2010-09-18 11:51:42 grahn Exp $
 *
 * Copyright (c) 2010 Jörgen Grahn
 * All rights reserved.
 *
 */
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <algorithm>

#include <cstdlib>
#include <getopt.h>

#include "library.h"
#include "worldfile.h"
#include "md5pp.h"

namespace {

    struct fmt {
	explicit fmt(double v) : val(v) {}
	double val;
    };

    std::ostream& operator<< (std::ostream& os, const fmt& val)
    {
	char buf[30];
	std::sprintf(buf, "%.2f", val.val);
	return os << buf;
    }

    std::string md5sum(const std::string& file)
    {
	std::ifstream is(file.c_str());
	md5::Ctx ctx;
	return ctx.update(is).digest().hex();
    }

    template<class Container, class Value>
    bool contains(const Container& c, const Value& value)
    {
	return std::find(c.begin(), c.end(), value) != c.end();
    }


    void describe_maps(const std::string& libfile,
		       const char* const * argv)
    {}


    void delta_maps(const std::string& libfile,
		    const std::string& dstfile,
		    const char* const * argv)
    {}
}


int main(int argc, char ** argv)
{
    using std::string;

    const string prog = argv[0];
    const string usage = string("usage: ")
	+ prog + " [-f mapping-file] map-file ...\n"
	+ "       "
	+ prog + " [-f mapping-file] -d destination-map source-map ...";
    const char optstring[] = "+f:d:h";
    struct option long_options[] = {
	{"version", 0, 0, 'v'},
	{"help", 0, 0, 'h'},
	{0, 0, 0, 0}
    };

    std::cin.sync_with_stdio(false);
    std::cout.sync_with_stdio(false);

    string libfile;
    string dstfile;

    int ch;
    while((ch = getopt_long(argc, argv,
			    optstring, &long_options[0], 0)) != -1) {
	switch(ch) {
	case 'f':
	    libfile = optarg;
	    break;
	case 'd':
	    dstfile = optarg;
	    break;
	case 'h':
	    std::cout << usage << '\n';
	    return 0;
	case 'v':
	    std::cout << prog << "\n$Name:  $\n"
		      << "Copyright (c) 2010 Jörgen Grahn\n";
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

    if(optind == argc) {
	std::cerr << "error: required argyment missing\n";
	std::cerr << usage << '\n';
	return 1;
    }

    if(dstfile.empty()) {
	describe_maps(libfile, argv+optind);
    }
    else {
	delta_maps(libfile, dstfile, argv+optind);
    }

    return 0;
}
