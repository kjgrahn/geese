/* -*- c++ -*-
 *
 * $Id: point.h,v 1.1 2010-08-26 18:59:23 grahn Exp $
 *
 * Copyright (c) 2003, 2004, 2010 Jörgen Grahn <grahn+src@snipabacken.se>
 * All rights reserved.
 */
#ifndef VECTOR_H
#define VECTOR_H

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

private:
    const double _x;
    const double _y;
};

std::ostream& operator<< (std::ostream& os, const Point& val);

#endif
