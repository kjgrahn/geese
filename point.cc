/*
 * $Id: point.cc,v 1.7 2010-08-29 18:59:32 grahn Exp $
 *
 * Copyright (c) 2010 Jörgen Grahn <grahn+src@snipabacken.se>
 * All rights reserved.
 */
#include "point.h"

#include <iostream>
#include <cstring>
#include <cmath>


std::ostream& operator<< (std::ostream& os, const SinCos& val)
{
    char buf[8];
    std::sprintf(buf, "%.4f", val.sin);
    os << "(sin=" << buf << ", ";
    std::sprintf(buf, "%.4f", val.cos);
    return os << "cos=" << buf << ')';
}


double Point::len() const
{
    return std::sqrt(x*x + y*y); 
}


SinCos Point::sincos() const
{
    const double n = len();
    return SinCos(y/n, x/n);
}


Point Point::operator- (const Point& other) const
{
    return Point(x-other.x, y-other.y);
}


/**
 * Print as e.g. "(1.004571e+06, -3.00000e+01)".
 */
std::ostream& Point::put(std::ostream& os) const
{
    char buf[8+6];
    std::sprintf(buf, "%.6e", x);
    os << '(' << buf << ", ";
    std::sprintf(buf, "%.6e", y);
    return os << buf << ')';
}

std::ostream& operator<< (std::ostream& os, const Point& val)
{
    return val.put(os);
}
