/*
 * $Id: vector.cc,v 1.2 2003-01-04 16:09:29 grahn Exp $
 *
 * vector.cc
 *
 * Copyright (c) 2003 Jörgen Grahn <jgrahn@algonet.se>
 * All rights reserved.
 */

#include <iostream>
#include <cmath>

#include "vector.h"


double Vector::length() const
{
    return std::sqrt(_x*_x + _y*_y);
}


double Vector::angle() const
{
    return std::atan2(_x, _y);
}


Vector operator+ (const Vector& a, const Vector& b)
{
    return Vector(a.x() + b.x(), a.y() + b.y());
}


Vector operator- (const Vector& a, const Vector& b)
{
    return Vector(a.x() - b.x(), a.y() - b.y());
}


std::ostream& operator<< (std::ostream& o, const Vector& v)
{
    o << "(" << int(v.x()) << ", " << int(v.y()) << ")";
    return o;
}
