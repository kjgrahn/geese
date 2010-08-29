/* -*- c++ -*-
 *
 * $Id: transform.h,v 1.11 2010-08-29 16:39:01 grahn Exp $
 *
 * transform.h
 *
 * Copyright (c) 2003, 2004, 2010 Jörgen Grahn <grahn+src@snipabacken.se>
 * All rights reserved.
 */
#ifndef GEESE_TRANSFORM_H
#define GEESE_TRANSFORM_H

#include "point.h"
#include <iosfwd>


/**
 * A coordinate in RT90.
 */
class RT90 {
public:
    RT90(double north, double east)
	: p(east, north)
    {}

    Point p;
};

std::ostream& operator<<  (std::ostream& os, const RT90& val);

inline double distance(const RT90& a, const RT90& b) {
    return distance(a.p, b.p);
}


/**
 * A coordinate in a raster image, with the usual origo
 * in the upper left corner.
 */
class Pixel {
public:
    Pixel(double x, double y)
	: p(x, y)
    {}

    Point p;
};

std::ostream& operator<<  (std::ostream& os, const Pixel& val);

inline double distance(const Pixel& a, const Pixel& b) {
    return distance(a.p, b.p);
}


/**
 * A transformation between the RT90 coordinate system and
 * coordinates in a raster image.
 *
 * This is really the six-parameter affine transformation
 * used in so called 'world files':
 *
 *   dst = AB src + C
 *         DE       F
 *
 * but the constructor which takes two pairs src/dst coordinates
 * defines a subset where the transformation is just a uniform
 * scaling, a rotation and a translation:
 *
 *   dst = S R src + T
 *
 * (plus the reflection of the y axis in raster images).
 */
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
    RT90 operator() (const Pixel& src) const;

private:
    double A;
    double B;
    double C;
    double D;
    double E;
    double F;
};

#endif
