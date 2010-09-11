/* -*- c++ -*-
 *
 * $Id: library.h,v 1.2 2010-09-11 15:24:19 grahn Exp $
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
    double width;
    double height;
};

struct Map {
    Map();
    Map(const std::vector<std::string>& checksums_,
	const Dimensions& dimensions_,
	const Transform& transform)
	: checksums(checksums_),
	dimensions(dimensions_),
	t(new Transform(transform))
    {}
    Map(const Map& other);
    ~Map();

    std::vector<std::string> checksums;
    Dimensions dimensions;
    const Transform* t;
};

typedef std::map<std::string, Map> Library;

Library parse_lib(const std::string& libfile, std::ostream& log);

#endif
