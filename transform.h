/* -*- c++ -*-
 *
 * $Id: transform.h,v 1.8 2010-08-27 22:26:27 grahn Exp $
 *
 * transform.h
 *
 * Copyright (c) 2003, 2004, 2010 Jörgen Grahn <grahn+src@snipabacken.se>
 * All rights reserved.
 */
#ifndef GEESE_TRANSFORM_H
#define GEESE_TRANSFORM_H

#include "point.h"


class RT90 {
public:
    RT90(double north, double east)
	: p_(east, north)
    {}

    double distance(const RT90& other) const;

private:
    Point p_;
};

inline double distance(const RT90& a, const RT90& b) {
    return a.distance(b);
}


class Pixel {
public:
    Pixel(double x, double y)
	: p_(x, y)
    {}

    double distance(const Pixel& other) const;

private:
    Point p_;
};

inline double distance(const Pixel& a, const Pixel& b) {
    return a.distance(b);
}


class Transform {
public:
    Transform(const RT90& src_a, const Pixel& dst_a,
	      const RT90& src_b, const Pixel& dst_b);

    Pixel& operator() (const RT90& src) const;
};


#endif
