/*
 * $Id: vector.cc,v 1.3 2003-01-05 18:35:24 grahn Exp $
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
    return std::atan2(_y, _x);
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
    o << double(v.x()) << " " << double(v.y()) ;
    return o;
}
