/* Copyright (c) 2010, 2021 Jörgen Grahn
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
#include "xv.h"
#include "canvas.h"
#include "md5pp.h"
#include "split.h"
#include "version.h"

namespace {

    /**
     * Given an image, a coordinate system on it, and a command line,
     * put dots in the image according to commands.
     */
    class Plot {
    public:
	Plot(const Transform& t, const std::string& mapfile)
	    : t {t},
	      canvas {mapfile}
	{}

	int run(std::istream& is, std::ostream& os);

    private:
	void add(const RT90& coord);
	void bullseye(const std::string& color, unsigned width);
	void show() const;

	const Transform& t;
	Canvas canvas;
    };

    bool in(const std::string& needle, const std::vector<std::string>& haystack)
    {
	auto it = std::find(begin(haystack), end(haystack), needle);
	return it != end(haystack);
    }

    /* Assuming all of 's' is a decimal number, return it.
     * Otherwise return 0.
     */
    unsigned get_n(const std::string& s)
    {
	const char* const p = s.c_str();
	char* end;
	unsigned n = std::strtoul(p, &end, 10);
	if (*end) return 0;
	return n;
    }

    RT90 get_coordinate(const std::string& s)
    {
	const char* p = s.c_str();
	char* end;
	unsigned n = std::strtoul(p, &end, 10);
	if (end==p || !*end) return {0, 0};
	p = end;
	unsigned e = std::strtoul(p, &end, 10);
	if (*end) return {0, 0};

	while (n < 1000000) {
	    n *= 10;
	    e *= 10;
	}

	return {n, e};
    }

    int Plot::run(std::istream& is, std::ostream& os)
    {
	static const char usage[] =
	    "usage: north-coord east-coord\n"
	    "       show\n"
	    "       bullseye white|black width\n"
	    "       help\n"
	    "       exit";

	std::string s;
	while (getline(is, s)) {
	    const auto ss = split(s);
	    if (ss.empty()) continue;

	    const auto& s0 = ss[0];
	    if (s0=="show") {
		show();
	    }
	    else if (s0=="bullseye") {

		unsigned width;
		if (ss.size()==3 && in(ss[1], {"white", "black"})
		    && (width = get_n(ss[2]))) {
		    bullseye(ss[1], width);
		}
		else {
		    os << usage << '\n';
		    continue;
		}
	    }
	    else if (s0=="exit") {
		break;
	    }
	    else if (in(s0, {"h", "help", "?"})) {
		os << usage << '\n';
	    }
	    else {
		const RT90 p = get_coordinate(s);
		if (!valid(p)) {
		    os << "Uh? You said '" << s << "'\n"
		       << usage << '\n';
		    continue;
		}
		add(p);
	    }
	}
	return 0;
    }

    void Plot::add(const RT90& coord)
    {
	const Pixel px = t(coord);
	canvas.plot(px);
    }

    void Plot::bullseye(const std::string& color, unsigned width)
    {
	canvas.bullseye(color, width);
    }

    void Plot::show() const
    {
	std::cout << "displaying ... " << std::flush;
	xv::Sink xv;
	canvas.write(xv);
	std::cout << "ok. Close viewer to continue.\n" << std::flush;
	xv.wait();
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
}

int main(int argc, char ** argv)
{
    using std::string;

    const string prog = argv[0] ? argv[0] : "geese_plot";
    const string usage = string("usage: ")
	+ prog + " [-t format] -f mapping-file map-file\n"
	"       "
	+ prog + " --version\n"
	"       "
	+ prog + " --help";
    const char optstring[] = "+Vf:";
    struct option long_options[] = {
	{"version", 0, 0, 'V'},
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
	case 'V':
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
				     libfile, {},
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

    std::cout << prog << ": displaying map ...\n"
	      << "one pixel is " << fmt(scale) << " m wide\n"
	      << "the map covers " << fmt(area/1e6) << " km²\n"
	      << "and is rotated " << fmt(t.rotation()) << "°\n"
	      << t << '\n';

    return Plot {t, mapfile}.run(std::cin, std::cout);
}
