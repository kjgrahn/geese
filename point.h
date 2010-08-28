/* -*- c++ -*-
 *
 * $Id: point.h,v 1.5 2010-08-28 08:17:46 grahn Exp $
 *
 * Copyright (c) 2003, 2004, 2010 Jörgen Grahn <grahn+src@snipabacken.se>
 * All rights reserved.
 */
#ifndef GEESE_POINT_H
#define GEESE_POINT_H

#include <iosfwd>

/**
 * An angle v expressed as (sin v, cos v).
 * It's up to the user to keep it honest.
 */
struct SinCos {
    SinCos(double s, double c): sin(s), cos(c) {}
    double sin;
    double cos;
};

/**
 * A two-dimensional vector; a point in the Cartesian plane.
 *
 */
class Point {
public:
    Point(double x, double y) : x_(x), y_(y) {}

    double len() const;
    SinCos sincos() const;
    double sin() const { return sincos().sin; }
    double cos() const { return sincos().cos; }

    Point operator- (const Point& other) const;

    std::ostream& put(std::ostream& os) const;

private:
    const double x_;
    const double y_;
};

inline double distance(const Point& a, const Point& b) {
    return (a-b).len();
}

std::ostream& operator<< (std::ostream& os, const Point& val);

#endif
