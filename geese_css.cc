/* $Id: geese_css.cc,v 1.5 2011-07-21 20:27:02 grahn Exp $
 *
 * Copyright (c) 2011 Jörgen Grahn
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
#include "worldfile.h"
#include "md5pp.h"
#include "split.h"
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


    /* True if 'coordinate' may lie inside 'map'.
     * ("May" because maps aren't required to know their size.)
     * 
     */
    bool contains(const Map& map, const RT90& coord)
    {
	const Pixel px = map.t(coord);
	if(px.p.x < 0) return false;
	if(px.p.y < 0) return false;
	if(map.dimensions.empty()) return true;
	if(px.p.x > map.dimensions.width) return false;
	if(px.p.y > map.dimensions.height) return false;
	return true;
    }


    void plot(const Map& map, std::ostream& os,
	      const RT90& coord,
	      const std::string& label,
	      const std::string& title,
	      const std::string& url,
	      const std::string& cls)
    {
	if(!contains(map, coord)) return;
	const Pixel px = map.t(coord);
	const int x = px.p.x + .5;
	const int y = px.p.y + .5;

	os << "<a style=\"left: " << x << "px; top: " << y << "px;\"";

	if(!title.empty()) os << " title=\"" << title << "\"";
	if(!cls.empty()) os << " class=\"" << cls << "\"";
	if(!url.empty()) os << " href=\"" << url << "\"";

	os << '>' << label << "</a>\n";
    }


    bool plot(const Map& map, std::ostream& os, std::istream& labels)
    {
	using std::vector;
	using std::string;

	/* XXX I/O errors and parse errors need more work */

	string s;
	while(getline(labels, s)) {
	    const vector<string> ss = split(s);
	    if(ss.empty()) continue;
	    if(!ss[0].empty() && ss[0][0]=='#') continue;

	    if(ss.size()<3) return false;
	    char* end;
	    double north = std::strtod(ss[0].c_str(), &end);
	    if(*end) return false;
	    double east = std::strtod(ss[1].c_str(), &end);
	    if(*end) return false;
	    /* XXX this task should be a library function */
	    while(north < 1e6) {
                north *= 10;
                east *= 10;
	    }
	    const string label = ss[2];
	    if(label.empty()) return false;
	    string title;
	    string url;
	    string cls;
	    if(ss.size()>3) title = ss[3];
	    if(ss.size()>4) url = ss[4];
	    if(ss.size()>5) cls = ss[5];

	    plot(map, os, RT90(north, east), label, title, url, cls);
	}

	return true;
    }
}


int main(int argc, char ** argv)
{
    using std::string;

    const string prog = argv[0];
    const string usage = string("usage: ")
	+ prog + " [-f mapping-file] map-file [labels]";
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

    if(optind == argc) {
	std::cerr << "error: too few arguments\n";
	std::cerr << usage << '\n';
	return 1;
    }

    const string mapfile = argv[optind++];

    const Map mapping = find_mapping(mapfile,
				     libfile, "",
				     std::cerr);
    if(mapping.empty) {
	std::cerr << "No mapping found for \"" << mapfile << "\": exiting\n";
	return 1;
    }
    if(!mapping.checksums.empty()) {

	const std::string digest = md5sum(mapfile);
	if(!contains(mapping.checksums, digest)) {
	    std::cerr << "error: " << mapfile
		      << ": bad checksum\n";
	    return 1;
	}
    }

    if(optind == argc) {
	plot(mapping, std::cout, std::cin);
    }

    while(optind < argc) {
	const std::string labelfile = argv[optind++];
	std::ifstream labels(labelfile.c_str());
	plot(mapping, std::cout, labels);
	labels.close();
    }

    return 0;
}
