/* -*- c++ -*-
 *
 * $Id: vector.h,v 1.5 2004-07-11 20:15:59 grahn Exp $
 *
 * vector.h
 *
 * Copyright (c) 2003, 2004 Jörgen Grahn <jgrahn@algonet.se>
 * All rights reserved.
 */
#ifndef VECTOR_H
#define VECTOR_H

#include <iosfwd>

/**
 * A two-dimensional vector; a point in a plane.
 *
 */
class Vector
{
public:
    explicit Vector(double x = 0.0, double y = 0.0) : _x(x), _y(y) {}

    double x() const {return _x;}
    double y() const {return _y;}

    double length() const;
    double angle() const;

private:
    const double _x;
    const double _y;
};


Vector operator+ (const Vector& a, const Vector& b);
Vector operator- (const Vector& a, const Vector& b);

inline double angle(const Vector& a, const Vector& b) {
    return (b-a).angle();
}

inline double distance(const Vector& a, const Vector& b) {
    return (b-a).length();
}

std::ostream& operator<< (std::ostream& o, const Vector& v);

#endif
