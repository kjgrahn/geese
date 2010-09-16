/* -*- c++ -*-
 *
 * $Id: transform.h,v 1.21 2010-09-16 21:44:06 grahn Exp $
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
    RT90(double north, double east) : p(east, north) {}
    explicit RT90(const Point& pp) : p(pp) {}
    Point p;

    struct Format {
	Format(unsigned n, const RT90& p) : n_(n), p_(p) {}
	const unsigned n_;
	const RT90& p_;
    };

    Format fmt(unsigned n) const { return Format(n, *this); }
};

std::ostream& operator<< (std::ostream& os, const RT90& val);

inline double distance(const RT90& a, const RT90& b) {
    return distance(a.p, b.p);
}

std::ostream& operator<< (std::ostream& os, const RT90::Format f);


/**
 * A coordinate in a raster image, with the usual origo
 * in the upper left corner.
 */
class Pixel {
public:
    Pixel(double x, double y) : p(x, y) {}
    explicit Pixel(const Point& pp) : p(pp) {}
    Point p;
};

std::ostream& operator<< (std::ostream& os, const Pixel& val);

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
 * Note that 'world files' describe the transformation from map to world,
 * i.e. from src Pixel to dst RT90. I call that the "out" transform,
 * and I call the inverse (world to map) the "in" transformation.
 *
 * On the other hand, the constructor which takes two pairs src/dst
 * coordinates defines a subset where the transformation is just a
 * uniform scaling, a rotation and a translation:
 *
 *   dst = S R src + T
 *
 * (plus the reflection of the y axis in raster images).
 */
class Transform {
private:
    struct Srrt {
	Srrt();
	Srrt(double a, double b, double c,
	     double d, double e, double f);
	Srrt(const Point& src_a, const Point& dst_a,
	     const Point& src_b, const Point& dst_b);
	Point operator() (const Point& src) const;
	std::ostream& put(std::ostream& os) const;
	double A;
	double B;
	double C;
	double D;
	double E;
	double F;
    };

    Srrt in_;
    Srrt out_;

public:
    Transform(const RT90& src_a, const Pixel& dst_a,
	      const RT90& src_b, const Pixel& dst_b);

    Transform(double a, double b, double c,
	      double d, double e, double f);

    Transform() {}

    double scale() const;
    double rotation() const;

    Pixel in(const RT90& src) const { return Pixel(in_(src.p)); }
    RT90 out(const Pixel& src) const { return RT90(out_(src.p)); }

    Pixel operator() (const RT90& src) const { return in(src); }
    RT90 operator() (const Pixel& src) const { return out(src); }

    std::ostream& put(std::ostream& os) const;
    std::ostream& worldfile(std::ostream& os) const;
};

inline std::ostream& operator<<  (std::ostream& os, const Transform& val)
{
    return val.put(os);
}

#endif
