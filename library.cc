/* $Id: library.cc,v 1.16 2011-06-14 21:59:48 grahn Exp $
 *
 * Copyright (c) 2010, 2011 J�rgen Grahn
 * All rights reserved.
 *
 */
#include "library.h"
#include "parse.h"
#include "files...h"
#include "worldfile.h"
#include "globbing.h"

#include <cstdlib>
#include <cctype>
#include <iostream>
#include <fstream>
#include <cerrno>
#include <cstring>
#include <cstdlib>


using std::vector;
using std::string;

typedef std::vector<std::string> Lines;

namespace {

    struct Mapping {
	double a;
	double b;
	double c;
	double d;
    };

    bool parse(const std::string& s, Dimensions& val)
    {
	const char* p = s.c_str();

	try {
	    val.width = eat<unsigned>(p);
	    eat(p, "x");
	    val.height = eat<unsigned>(p);
	    eat_ws(p);
	    return *p=='\0';
	}
	catch (const ParseError&) {
	    return false;
	}
    }

    bool parse(const std::string& s, Mapping& val)
    {
	const char* p = s.c_str();

	try {
	    val.a = eat<double>(p);
	    val.b = eat<double>(p);
	    eat(p, "->");
	    val.c = eat<double>(p);
	    eat_optional(p, ",");
	    val.d = eat<double>(p);
	    eat_ws(p);
	    return *p=='\0';
	}
	catch (const ParseError&) {
	    return false;
	}
    }

    bool parse(const std::string& s, md5::Digest& val)
    {
	const md5::Digest nil;
	val = md5::parse(s);
	return val != nil;
    }

    void parse(Library<Map>& lib,
	       const Lines& acc,
	       const Files::Position& pos,
	       std::ostream& log)
    {
	std::vector<std::string> names;
	std::vector<md5::Digest> checksums;
	Dimensions dim;
	std::vector<Mapping> mappings;

	for (const std::string& s: acc) {
	    if (dim.empty()) {
		if (parse(s, dim)) continue;
		md5::Digest md5;
		if (parse(s, md5)) {
		    names.push_back(md5.hex());
		    checksums.push_back(md5);
		    continue;
		}
		names.push_back(s);
		continue;
	    }
	    else {
		Mapping m;
		if (parse(s, m)) {
		    mappings.push_back(m);
		    continue;
		}
		log << pos << ": warning: ignoring line \"" << s << "\"\n";
	    }
	}

	if(names.empty()) {
	    log << pos << ": " << "warning: entry without file name discarded\n";
	    return;
	}
	if(mappings.size()<2) {
	    log << pos << ": " << "warning: entry without mappings discarded\n";
	    return;
	}
	if(mappings.size()>2) {
	    log << pos << ": " << "warning: ignoring extra mappings (I only use two)\n";
	}

	const Mapping& a = mappings[0];
	const Mapping& b = mappings[1];

	const Map map(checksums, dim,
		      Transform(RT90(a.a, a.b), Pixel(a.c, a.d),
				RT90(b.a, b.b), Pixel(b.c, b.d)));

	for (const auto& name: names) {

	    if (lib.count(name)) {
		log << pos << ": warning: ignoring extra entry for '"
		    << name << "'\n";
		continue;
	    }
	    lib.emplace(name, map);
	}
    }

}


bool Dimensions::empty() const
{
    /* I assume 0.0==0.0 */
    const double zero = 0;
    return width==zero || height==zero;
}


/**
 * Read a map "library" from 'files', printing errors/warnings to 'log'.
 * Returns an empty library in case of complete failure.
 */
Library<Map> parse_lib(Files& files, std::ostream& log)
{
    Library<Map> lib;

    Lines acc;
    std::string s;

    while(files.getline(s)) {
	if(!s.empty() && s[0] == '#') {
	    continue;
	}

	if(s.empty()) {
	    if(!acc.empty()) {
		parse(lib, acc, files.position(), log);
		acc.clear();
	    }
	    continue;
	}

	acc.push_back(s);
    }

    if(!acc.empty()) {
	parse(lib, acc, files.position(), log);
    }

    return lib;
}


/**
 * Kind of like POSIX basename(3), but with certain limitations.
 */
std::string basename(const std::string& path)
{
    std::string::size_type n = path.rfind('/');
    if(n==std::string::npos) {
	return path;
    }
    return std::string(path, n+1);
}

namespace {

    string::size_type extension(const std::string& path)
    {
	string::size_type n = path.find_last_of("./");
	if(n==string::npos || path[n]=='/') return string::npos;
	return n;
    }

    /**
     * "foo" -> "�/foo"
     * "foo/bar" -> "foo/�/bar"
     */
    std::string star(const std::string& path)
    {
	string p = path;
	string::size_type n = p.rfind('/');
	if(n==string::npos) {
	    return p.insert(0, "*/");
	}
	return p.insert(n, "/*");
    }

    std::string barw(const std::string& path)
    {
	return path + "w";
    }

    std::string brw(const std::string& path)
    {
	string p = path;
	string::size_type n = extension(p);
	if(n==string::npos) return "";
	if(n+4!=p.size()) return "";
	return p.erase(n+2, 1) + "w";
    }

    std::string wld(const std::string& path)
    {
	string p = path;
	string::size_type n = extension(p);
	if(n==string::npos) return "";
	return p.replace(n, string::npos, ".wld");
    }

    std::string tfw(const std::string& path)
    {
	string p = path;
	string::size_type n = extension(p);
	if(n==string::npos) return "";
	return p.replace(n, string::npos, ".tfw");
    }
}


/**
 * Find the mapping for image file 'mapfile', if one exists.  Mostly
 * the transform, but also MD5sum and dimensions (if found in
 * 'lib'). Also log I/O errors and warnings to 'log' (but doesn't log
 * "file not found" since there are no guarantees that one /does/
 * exist.)
 *
 * The search order is (let's pretend 'mapfile' is 'mapfile.bar'):
 * - only 'worldfile' is searched if it's given
 * - 'libfile', a geese mapping file
 * - world files named
 *   - mapfile.wld
 *   - �/mapfile.wld
 *   - mapfile.barw
 *   - �/mapfile.barw
 *   - mapfile.brw
 *   - �/mapfile.brw
 *   - mapfile.tfw (because the GIS community has a TIFF fetish)
 *   - �/mapfile.tfw
 *
 */
Map find_mapping(const string& mapfile,
		 const Library<Map>& library,
		 const string& worldfile,
		 std::ostream& log)
{
    Map mapping;

    if(!worldfile.empty()) {
	mapping.empty = !parse_world(mapping.t, worldfile, log);
	return mapping;
    }

    if(!library.empty()) {
	const auto i = library.find(basename(mapfile));
	if(i!=library.end()) {
	    mapping = i->second;
	    return mapping;
	}
    }

    vector<string> wnames;
    wnames.push_back(wld(mapfile));
    glob(star(wld(mapfile)), wnames);
    wnames.push_back(barw(mapfile));
    glob(star(barw(mapfile)), wnames);
    const string brw = ::brw(mapfile);
    if(!brw.empty()) {
	wnames.push_back(brw);
	glob(star(brw), wnames);
    }
    const string tfw = ::tfw(mapfile);
    if(!tfw.empty()) {
	wnames.push_back(tfw);
	glob(star(tfw), wnames);
    }

    for(vector<string>::const_iterator i=wnames.begin(); i!=wnames.end(); ++i) {

	mapping.empty = !parse_world(mapping.t, *i, log);
	if(!mapping.empty) break;
    }

    return mapping;
}


Map find_mapping(const std::string& mapfile,
		 const std::string& libfile,
		 const std::string& worldfile,
		 std::ostream& log)
{
    const auto a = &libfile;
    const auto b = a+1;
    Files files {a, b, false};
    return find_mapping(mapfile,
			parse_lib(files, log),
			worldfile,
			log);
}
