/* $Id: geese_fit.cc,v 1.1 2010-12-12 21:40:19 grahn Exp $
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


    void describe_maps(const Library& lib,
		       const char* const * argv)
    {
	using std::cout;

	unsigned i = 0;
	while(const char* const p = *argv++) {
	    const std::string f(p);

	    const Map m = find_mapping(f, lib, "", std::cerr);
	    if(m.empty) {
		std::cerr << f << ": no coordinate info found\n";
		continue;
	    }

	    const std::string digest = md5sum(f);

	    if(!m.checksums.empty() &&
	       !contains(m.checksums, digest)) {
		std::cout << "error: " << f << ": bad checksum!\n";
		continue;
	    }

	    if(i++) {
		cout << '\n';
	    }
	    cout << f << '\n';
	    for(std::vector<std::string>::const_iterator i = m.checksums.begin();
		i != m.checksums.end();
		++i) {
		cout << *i << '\n';
	    }
	    if(m.checksums.empty()) {
		cout << digest << '\n';
	    }

	    const Transform& t = m.t;
	    const Pixel da(0, 0);
	    const Pixel db(1000, 0);

	    cout << t(da) << " -> " << da << '\n'
		 << t(db) << " -> " << db << '\n';

	    t.worldfile(cout);
	    cout << '\n';
	}
    }


    void delta_maps(const Library& lib,
		    const std::string& dstfile,
		    const char* const * argv)
    {
	using std::cout;

	const Map ref = find_mapping(dstfile, lib, "", std::cerr);
	if(ref.empty) {
	    std::cerr << dstfile << ": no coordinate info found\n";
	    return;
	}

	while(const char* const p = *argv++) {
	    const std::string f(p);

	    const Map m = find_mapping(f, lib, "", std::cerr);
	    if(m.empty) {
		std::cerr << f << ": no coordinate info found\n";
		continue;
	    }

	    const double scale = m.t.scale()/ref.t.scale();
	    const double rot = ref.t.rotation() - m.t.rotation();

	    cout << f << ": scale by " << scale << " (" << fmt(scale*1e2)
		 << " %); rotate " << fmt(rot) << "°\n";
	}
    }
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

    Library lib;
    if(!libfile.empty()) {
	lib = parse_lib(libfile, std::cerr);
	if(lib.empty()) return 1;
    }

    if(dstfile.empty()) {
	describe_maps(lib, argv+optind);
    }
    else {
	delta_maps(lib, dstfile, argv+optind);
    }

    return 0;
}
