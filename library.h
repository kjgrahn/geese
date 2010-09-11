/* -*- c++ -*-
 *
 * $Id: library.h,v 1.4 2010-09-11 18:15:05 grahn Exp $
 *
 * Copyright (c) 2010 Jörgen Grahn <grahn+src@snipabacken.se>
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
    Map() {}
    Map(const std::vector<std::string>& checksums_,
	const Dimensions& dimensions_,
	const Transform& transform)
	: checksums(checksums_),
	  dimensions(dimensions_),
	  t(transform)
    {}

    std::vector<std::string> checksums;
    Dimensions dimensions;
    Transform t;
};

typedef std::map<std::string, Map> Library;

Library parse_lib(const std::string& libfile, std::ostream& log);

#endif
