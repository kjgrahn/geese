/* $Id: geese_ref.cc,v 1.4 2010-09-18 11:19:48 grahn Exp $
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
#include "child.h"
#include "xvpixel.h"
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

    /**
     * Georeference 'map' against 'refmap', which itself has the
     * transform 't'.  Make several rounds, if the user so chooses.
     */
    void reference(const Transform& t,
		   const std::string& refmap,
		   const std::string& map)
    {
	using std::cout;

	const std::string checksum = md5sum(map);

	const char* xvargs[] = { "xv", refmap.c_str(), 0 };
	Child rxv(const_cast<char**>(xvargs));
	xvargs[1] = map.c_str();
	Child xv(const_cast<char**>(xvargs));

	cout << "Start pasting coordinates, pair by pair.\n"
	     << "The reference map's first point, then the\n"
	     << "same point on your target map.\n";

	std::vector<Pixel> pp;

	std::string s;
	while(std::getline(std::cin, s)) {
	    bool err = false;
	    const Pixel pixel = xvpixel(s, err);
	    if(err) {
		cout << "No pixel on that line; ignored.\n";
		continue;
	    }

	    pp.push_back(pixel);

	    if(pp.size() % 2) {
		cout << "And now the corresponding target map point ...\n";
		continue;
	    }

	    if(pp.size()==2) {
		cout << "And now a second pair of points ...\n";
		continue;
	    }

	    if(pp.size()==4) {
		/* { srcA dstA srcB dstB } */
		const RT90 sa = t(pp[0]);
		const Pixel da = pp[1];
		const RT90 sb = t(pp[2]);
		const Pixel db = pp[3];

		const Transform tb(sa, da, sb, db);

		cout << '\n'
		     << map << '\n'
		     << checksum << '\n'
		     << sa << " -> " << da << '\n'
		     << sb << " -> " << db << '\n';

		cout << '\n';
		tb.worldfile(cout);
		cout << '\n';

		pp.erase(pp.begin(), pp.begin()+2);
	    }
	}
    }
}


int main(int argc, char ** argv)
{
    using std::string;

    const string prog = argv[0];
    const string usage = string("usage: ")
	+ prog + " [-f mapping-file] reference-map map";
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

    if(optind+2 != argc) {
	std::cerr << "error: extra or too few arguments\n";
	std::cerr << usage << '\n';
	return 1;
    }

    const string refmapfile = argv[optind++];
    const string mapfile = argv[optind++];

    const Map refmapping = find_mapping(refmapfile,
					libfile, "",
					std::cerr);
    if(refmapping.empty) {
	std::cerr << "No mapping found for \"" << refmapfile << "\": exiting\n";
	return 1;
    }

    if(!refmapping.checksums.empty()) {

	const std::string digest = md5sum(refmapfile);
	if(!contains(refmapping.checksums, digest)) {
	    std::cerr << "warning: " << refmapfile
		      << ": bad MD5 checksum; aborting\n";
	    return 1;
	}
    }

    const Transform& rt = refmapping.t;
    const double scale = rt.scale();
    const double area = scale * refmapping.dimensions.width
	              * scale * refmapping.dimensions.height; 

    std::cout << "geese_pick: displaying map ...\n"
	      << "one pixel is " << fmt(scale) << " m wide\n"
	      << "the map covers " << fmt(area/1e6) << " km²\n"
	      << "and is rotated " << fmt(rt.rotation()) << "°\n"
	      << rt << '\n';

    reference(rt, refmapfile, mapfile);

    return 0;
}
