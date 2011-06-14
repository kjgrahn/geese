/*
 * $Id: worldfile.cc,v 1.5 2011-06-14 21:32:38 grahn Exp $
 *
 * Copyright (c) 2010 Jörgen Grahn <grahn+src@snipabacken.se>
 * All rights reserved.
 */
#include "worldfile.h"
#include "transform.h"

#include <iostream>
#include <fstream>
#include <string>
#include <vector>

#include <cerrno>
#include <cstdlib>
#include <cstring>

bool parse_world(Transform& t, const std::string& file, std::ostream& log)
{
    std::ifstream is(file.c_str());
    if(is.fail()) {
	if(errno!=ENOENT) {
	    log << "error: cannot open " << file << ": "
		<< std::strerror(errno) << '\n';
	}
	return false;
    }

    std::vector<double> acc;
    std::string s;

    while(getline(is, s)) {
	const char* p = s.c_str();
	char* end;
	acc.push_back(std::strtod(p, &end));
	const char* q = end;
	while(isspace(*q)) ++q;
	if(end==p || *q) {
	    log << "error: " << file << ": not a world file\n";
	    return false;
	}
    }

    if(!is.eof()) {
	log << "error reading " << file << ": "
	    << std::strerror(errno) << '\n';
	return false;
    }
    if(acc.size() != 6) {
	log << "error: " << file << ": not a world file\n";
	return false;
    }

    t = Transform(acc[0],acc[2],acc[4],
		  acc[1],acc[3],acc[5]);
    return true;
}
