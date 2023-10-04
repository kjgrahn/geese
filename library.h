/* -*- c++ -*-
 *
 * $Id: library.h,v 1.8 2011-06-14 21:59:48 grahn Exp $
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
#include "md5pp.h"

struct Dimensions {
    Dimensions() : width(0), height(0) {}
    double width;
    double height;
    bool empty() const;
};

struct Map {
    Map() : empty(true) {}
    Map(const std::vector<md5::Digest>& checksums_,
	const Dimensions& dimensions_,
	const Transform& transform)
	: empty(false),
	  checksums(checksums_),
	  dimensions(dimensions_),
	  t(transform)
    {}

    bool empty;
    std::vector<md5::Digest> checksums;
    Dimensions dimensions;
    Transform t;
};

template <class T>
using Library = std::map<std::string, T>;

class Files;
Library<Map> parse_lib(Files& files, std::ostream& log);

Map find_mapping(const std::string& mapfile,
		 const Library<Map>& library,
		 const std::string& worldfile,
		 std::ostream& log);

Map find_mapping(const std::string& mapfile,
		 const std::string& libfile,
		 const std::string& worldfile,
		 std::ostream& log);

std::string basename(const std::string& path);

#endif
