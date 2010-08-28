/*
 * $Id: point.cc,v 1.5 2010-08-28 08:17:46 grahn Exp $
 *
 * Copyright (c) 2010 Jörgen Grahn <grahn+src@snipabacken.se>
 * All rights reserved.
 */
#include "point.h"

#include <iostream>
#include <cstring>
#include <cmath>

double Point::len() const
{
    return std::sqrt(x_*x_ + y_*y_); 
}


SinCos Point::sincos() const
{
    const double n = len();
    return SinCos(y_/n, x_/n);
}


Point Point::operator- (const Point& other) const
{
    return Point(x_-other.x_, y_-other.y_);
}


/**
 * Print as e.g. "(1.004571e+06, -3.00000e+01)".
 */
std::ostream& Point::put(std::ostream& os) const
{
    char buf[8+6];
    std::sprintf(buf, "%.6e", x_);
    os << '(' << buf << ", ";
    std::sprintf(buf, "%.6e", y_);
    return os << buf << ')';
}

std::ostream& operator<< (std::ostream& os, const Point& val)
{
    return val.put(os);
}
