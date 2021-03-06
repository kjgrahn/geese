/* -*- c++ -*-
 *
 * $Id: library.h,v 1.8 2011-06-14 21:59:48 grahn Exp $
 *
 * Copyright (c) 2010, 2011 J�rgen Grahn <grahn+src@snipabacken.se>
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
    bool empty() const;
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
