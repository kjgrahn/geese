/* $Id: geese_pick.cc,v 1.17 2011-02-05 16:27:51 grahn Exp $
 *
 * Copyright (c) 2010 Jörgen Grahn
 * All rights reserved.
 *
 */
#include <iostream>
#include <fstream>
#include <string>
#include <algorithm>

#include <cstdlib>
#include <getopt.h>

#include "library.h"
#include "child.h"
#include "xvpixel.h"
#include "worldfile.h"
#include "md5pp.h"
#include "version.h"

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
		std::cout << "    " << int(.5+distance(prev, coord))
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
	    std::cout << prog << ", part of "
		      << version::geese() << ' ' << version::version() << '\n'
		      << version::copyright() << '\n';
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

    if(optind+1 != argc) {
	std::cerr << "error: extra or too few arguments\n";
	std::cerr << usage << '\n';
	return 1;
    }

    const string mapfile = argv[optind];

    const Map mapping = find_mapping(mapfile,
				     libfile, worldfile,
				     std::cerr);
    if(mapping.empty) {
	std::cerr << "No mapping found for \"" << mapfile << "\": exiting\n";
	return 1;
    }
    if(!mapping.checksums.empty()) {

	const std::string digest = md5sum(mapfile);
	if(!contains(mapping.checksums, digest)) {
	    std::cout << "warning: " << mapfile
		      << ": bad checksum - distrust the coordinates!\n";
	}
    }

    const Transform& t = mapping.t;
    const double scale = t.scale();
    const double area = scale * mapping.dimensions.width
	              * scale * mapping.dimensions.height; 

    std::cout << "geese_pick: displaying map ...\n"
	      << "one pixel is " << fmt(scale) << " m wide\n"
	      << "the map covers " << fmt(area/1e6) << " km²\n"
	      << "and is rotated " << fmt(t.rotation()) << "°\n"
	      << t << '\n';

    pick(t, mapfile);

    return 0;
}
