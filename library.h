/* -*- c++ -*-
 *
 * $Id: library.h,v 1.7 2011-02-05 11:08:27 grahn Exp $
 *
 * Copyright (c) 2010, 2011 Jörgen Grahn <grahn+src@snipabacken.se>
 * All rights reserved.
 */
#ifndef GEESE_LIBRARY_H
#define GEESE_LIBRARY_H

#include <iosfwd>
#include <string>
#include <vector>
#include <map>

#include "transform.h"

struct Dimensions {
    Dimensions() : width(0), height(0) {}
    double width;
    double height;
};

struct Map {
    Map() : empty(true) {}
    Map(const std::vector<std::string>& checksums_,
	const Dimensions& dimensions_,
	const Transform& transform)
	: empty(false),
	  checksums(checksums_),
	  dimensions(dimensions_),
	  t(transform)
    {}

    bool empty;
    std::vector<std::string> checksums;
    Dimensions dimensions;
    Transform t;
};

typedef std::map<std::string, Map> Library;

Library parse_lib(const std::string& libfile, std::ostream& log);

Map find_mapping(const std::string& mapfile,
		 const Library& library,
		 const std::string& worldfile,
		 std::ostream& log);

Map find_mapping(const std::string& mapfile,
		 const std::string& libfile,
		 const std::string& worldfile,
		 std::ostream& log);

std::string basename(const std::string& path);

#endif
