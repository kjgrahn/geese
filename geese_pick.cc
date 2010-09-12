/* $Id: geese_pick.cc,v 1.11 2010-09-12 22:04:45 grahn Exp $
 *
 * Copyright (c) 2010 Jörgen Grahn
 * All rights reserved.
 *
 */
#include <iostream>
#include <iomanip>
#include <string>

#include <cstdlib>
#include <getopt.h>
#include <sys/types.h>

#include "library.h"
#include "child.h"
#include "xvpixel.h"
#include "worldfile.h"

namespace {

    std::string basename(const std::string& path)
    {
	std::string::size_type n = path.rfind('/');
	if(n==std::string::npos) {
	    return path;
	}
	return std::string(path, n+1);
    }


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


    /**
     * The core "picking" part.
     */
    void pick(const Transform& t, const std::string& mapfile)
    {
	const char* xvargs[] = { "xv", mapfile.c_str(), 0 };
	Child xv(const_cast<char**>(xvargs));

	int i = 0;
	RT90 prev(0,0);
	std::string s;
	while(std::getline(std::cin, s)) {
	    bool err = false;
	    const Pixel pixel = xvpixel(s, err);
	    if(err) continue;

	    const RT90 coord = t(pixel);

	    std::cout << "1m: " << coord.fmt(1)
		      << "; 10m: " << coord.fmt(10)
		      << "; 100m: " << coord.fmt(100)
		      << "; 1km: " << coord.fmt(1000) << '\n';
	    if(i++) {
		std::cout << "    " << distance(prev, coord)
			  << "m from previous coordinate\n";
	    }
	    prev = coord;
	}
    }
}


int main(int argc, char ** argv)
{
    using std::string;

    const string prog = argv[0];
    const string usage = string("usage: ")
	+ prog + " [-f mapping-file] map-file\n"
	"       "
	+ prog + " -w world-file map-file";
    const char optstring[] = "+f:w:h";
    struct option long_options[] = {
	{"version", 0, 0, 'v'},
	{"help", 0, 0, 'h'},
	{0, 0, 0, 0}
    };

    string libfile;
    string worldfile;

    std::cin.sync_with_stdio(false);
    std::cout.sync_with_stdio(false);

    int ch;
    while((ch = getopt_long(argc, argv,
			    optstring, &long_options[0], 0)) != -1) {
	switch(ch) {
	case 'f':
	    libfile = optarg;
	    break;
	case 'w':
	    worldfile = optarg;
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

    if((libfile.empty() && worldfile.empty())) {
	std::cerr << usage << '\n';
	return 1;
    }
    if(optind+1 != argc) {
	std::cerr << "error: extra or too few arguments\n";
	std::cerr << usage << '\n';
	return 1;
    }

    const string mapfile = argv[optind];

    Transform t;
    double area = 0;

    /* tedious ... a whole nest of alternatives and fallbacks */

    if(!worldfile.empty()) {
	if(!parse_world(t, worldfile, std::cerr)) {
	    return 1;
	}
    }
    else {
	const Library library = parse_lib(libfile, std::cerr);
	if(library.empty()) {
	    return 1;
	}
	const Library::const_iterator i = library.find(basename(mapfile));
	if(i!=library.end()) {
	    const Map& mapping = i->second;
	    t = mapping.t;
	    const double scale = t.scale();
	    area = scale * mapping.dimensions.width
		 * scale * mapping.dimensions.height; 
	}
	else {
	    if(!find_world(t, mapfile, std::cerr)) {
		std::cerr << "No mapping found for \"" << mapfile << "\": exiting\n";
		return 1;
	    }
	}
    }

    std::cout << "geese_pick: displaying map ...\n"
	      << "one pixel is " << fmt(t.scale()) << " m wide\n"
	      << "the map covers " << fmt(area/1e6) << " km²\n"
	      << "and is rotated " << fmt(t.rotation()) << "°\n"
	      << t << '\n';

    pick(t, mapfile);

    return 0;
}
