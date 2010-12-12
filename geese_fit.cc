/* $Id: geese_fit.cc,v 1.3 2010-12-12 23:28:37 grahn Exp $
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

    /* XXX merge with the one in library.cc */
    std::string basename(const std::string& path)
    {
	std::string::size_type n = path.rfind('/');
	if(n==std::string::npos) {
	    return path;
	}
	return std::string(path, n+1);
    }

    template<class Container, class Value>
    bool contains(const Container& c, const Value& value)
    {
	return std::find(c.begin(), c.end(), value) != c.end();
    }

    /**
     * The actual core of the utility, with arguments corresponding
     * to the command-line arguments.
     */
    void fit_maps(const Library& lib,
		  const std::string& targetdir,
		  const unsigned prescale,
		  char** argv)
    {
	using std::cout;

	const std::string base = *argv++;

	const Map ref = find_mapping(base, lib, "", std::cerr);
	if(ref.empty) {
	    std::cerr << base << ": no coordinate info found\n";
	    return;
	}

	const double pscale = prescale/1e6;

	if(prescale!=1e6) {
	    cout << "gm convert -scale " << pscale*100 << '%'
		 << " \"" << base << "\" "
		 << '\"' << targetdir << '/' << basename(base) << "\"\n";
	}
	else {
	    cout << "cp \"" << base << "\" targetdir\n";
	}

	while(const char* const p = *argv++) {
	    const std::string f(p);

	    const Map m = find_mapping(f, lib, "", std::cerr);
	    if(m.empty) {
		std::cerr << f << ": no coordinate info found\n";
		continue;
	    }

	    const double scale = m.t.scale()/ref.t.scale()*pscale;
	    const double rot = ref.t.rotation() - m.t.rotation();

	    cout << "gm convert"
		 << " -rotate " << rot
		 << " -scale " << scale*100 << '%'
		 << " \"" << f << "\" "
		 << '\"' << targetdir << '/' << basename(f) << "\"\n";
	}
    }
}


int main(int argc, char ** argv)
{
    using std::string;

    const string prog = argv[0];
    const string usage = string("usage: ")
	+ prog + " [-f mapping-file] [-s scale] -d target-dir map-file ...";
    const char optstring[] = "+f:s:d:h";
    struct option long_options[] = {
	{"version", 0, 0, 'v'},
	{"help", 0, 0, 'h'},
	{0, 0, 0, 0}
    };

    std::cin.sync_with_stdio(false);
    std::cout.sync_with_stdio(false);

    string libfile;
    string targetdir;
    string scale;
    unsigned prescale = 1e6;

    int ch;
    while((ch = getopt_long(argc, argv,
			    optstring, &long_options[0], 0)) != -1) {
	switch(ch) {
	case 'f':
	    libfile = optarg;
	    break;
	case 's':
	    scale = optarg;
	    break;
	case 'd':
	    targetdir = optarg;
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

    if(argc - optind < 2 || targetdir.empty()) {
	std::cerr << "error: required argyment missing\n";
	std::cerr << usage << '\n';
	return 1;
    }

    if(!scale.empty()) {
	/* just an early check */
	char* end;
	double s = std::strtod(scale.c_str(), &end);
	if(*end || s < 1e-3 || s > 1e3) {
	    std::cerr << "error: invalid -s argument\n";
	    std::cerr << usage << '\n';
	    return 1;
	}
	prescale = 1e6*(s+0.5e-6);
    }

    Library lib;
    if(!libfile.empty()) {
	lib = parse_lib(libfile, std::cerr);
	if(lib.empty()) return 1;
    }

    fit_maps(lib, targetdir, prescale, argv+optind);

    return 0;
}
