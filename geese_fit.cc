/* $Id: geese_fit.cc,v 1.6 2011-02-05 16:27:51 grahn Exp $
 *
 * Copyright (c) 2010, 2011 J�rgen Grahn
 * All rights reserved.
 *
 */
#include <iostream>
#include <fstream>
#include <string>

#include <cstdlib>
#include <getopt.h>

#include "library.h"
#include "files...h"
#include "version.h"

namespace {

    /**
     * The actual core of the utility, with arguments corresponding
     * to the command-line arguments.
     */
    void fit_maps(const Library<Map>& lib,
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
	+ prog + " [-f mapping-file] ... [-s scale] -d target-dir map-file ...";
    const char optstring[] = "f:s:d:h";
    struct option long_options[] = {
	{"version", 0, 0, 'v'},
	{"help", 0, 0, 'h'},
	{0, 0, 0, 0}
    };

    std::cin.sync_with_stdio(false);
    std::cout.sync_with_stdio(false);

    std::vector<string> libfile;
    string targetdir;
    string scale;
    unsigned prescale = 1e6;

    int ch;
    while((ch = getopt_long(argc, argv,
			    optstring, &long_options[0], 0)) != -1) {
	switch(ch) {
	case 'f':
	    libfile.emplace_back(optarg);
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

    if(argc - optind < 2 || targetdir.empty()) {
	std::cerr << "error: required argument missing\n";
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

    Library<Map> lib;
    if(!libfile.empty()) {
	Files files {begin(libfile), end(libfile), false};
	lib = parse_lib(files, std::cerr);
	if(lib.empty()) return 1;
    }

    fit_maps(lib, targetdir, prescale, argv+optind);

    return 0;
}
