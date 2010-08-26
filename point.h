/* -*- c++ -*-
 *
 * $Id: point.h,v 1.3 2010-08-26 21:19:56 grahn Exp $
 *
 * Copyright (c) 2003, 2004, 2010 J�rgen Grahn <grahn+src@snipabacken.se>
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
    Point(double x, double y) : _x(x), _y(y) {}

    double len() const;
    double sin() const;
    double cos() const;

    std::ostream& put(std::ostream& os) const;

private:
    const double _x;
    const double _y;
};

std::ostream& operator<< (std::ostream& os, const Point& val);

#endif
