/* -*- c++ -*-
 *
 * $Id: transform.h,v 1.10 2010-08-28 14:58:13 grahn Exp $
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
	: p(east, north)
    {}

    double distance(const RT90& other) const;
    Point p;
};

inline double distance(const RT90& a, const RT90& b) {
    return a.distance(b);
}


class Pixel {
public:
    Pixel(double x, double y)
	: p(x, y)
    {}

    double distance(const Pixel& other) const;
    Point p;
};

inline double distance(const Pixel& a, const Pixel& b) {
    return a.distance(b);
}


class Transform {
public:
    Transform(double a, double b, double c,
	      double d, double e, double f)
	: A(a), B(b), C(c),
	  D(d), E(e), F(f)
    {}

    Transform(const RT90& src_a, const Pixel& dst_a,
	      const RT90& src_b, const Pixel& dst_b);

    Pixel operator() (const RT90& src) const;

private:
    double A;
    double B;
    double C;
    double D;
    double E;
    double F;
};

#endif
