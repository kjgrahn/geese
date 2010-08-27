/* -*- c++ -*-
 *
 * $Id: point.h,v 1.4 2010-08-27 20:42:55 grahn Exp $
 *
 * Copyright (c) 2003, 2004, 2010 Jörgen Grahn <grahn+src@snipabacken.se>
 * All rights reserved.
 */
#ifndef GEESE_POINT_H
#define GEESE_POINT_H

#include <iosfwd>

/**
 * A two-dimensional vector; a point in the Cartesian plane.
 *
 */
class Point {
public:
    Point(double x, double y) : x_(x), y_(y) {}

    double len() const;
    double sin() const;
    double cos() const;

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
