/* $Id: geese_pick.cc,v 1.5 2010-09-11 23:52:35 grahn Exp $
 *
 * Copyright (c) 2010 J�rgen Grahn
 * All rights reserved.
 *
 */
#include <iostream>
#include <string>

#include <cstdlib>
#include <getopt.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <signal.h>
#include <errno.h>
#include <string.h>

#include "library.h"

namespace {

    std::string basename(const std::string& path)
    {
	std::string::size_type n = path.rfind('/');
	if(n==std::string::npos) {
	    return path;
	}
	return std::string(path, n+1);
    }

    struct Child {
	explicit Child(char** argv);
	~Child();
	int child;
    private:
	Child();
	Child(const Child&);
	Child& operator= (const Child&);
    };

    Child::Child(char** argv)
    {
	child = fork();
	if(child) return;

	execvp(argv[0], argv);
	std::cerr << "failed to exec '" << argv[0]
		  << "': " << strerror(errno) << '\n';
	exit(1);
    }

    Child::~Child()
    {
	if(child>0) {
	    kill(child, SIGINT);
	    int status;
	    wait(&status);
	}
    }

    /**
     * The core "picking" part.
     */
    void pick(const Transform& t, const std::string& mapfile)
    {
	const char* xvargs[] = { "xv", mapfile.c_str(), 0 };
	Child xv(const_cast<char**>(xvargs));

	std::string s;
	while(std::getline(std::cin, s)) {
	    /*  773, 846 = 252,254,2... */
	    const char* p = s.c_str();
	    char* end;
	    unsigned x = std::strtoul(p, &end, 10);
	    if(end==p || *end!=',') continue;
	    p = end + 1;
	    unsigned y = std::strtoul(p, &end, 10);
	    if(end==p || *end!=' ') continue;
	    p = end;
	    while(std::isspace(*p)) ++p;
	    if(*p != '=') continue;

	    const Pixel pixel(x, y);
	    std::cout << pixel << '\n';
	}
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

    Library::const_iterator i = library.find(basename(mapfile));
    if(i==library.end()) {
	std::cerr << "No mapping found for \"" << mapfile << "\": exiting\n";
	return 1;
    }

    const Transform& transform = i->second.t;

    pick(transform, mapfile);

    return 0;
}
