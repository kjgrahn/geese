/*
 * $Id: point.cc,v 1.2 2010-08-26 19:12:07 grahn Exp $
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
    return std::sqrt(_x*_x + _y*_y); 
}

double Point::sin() const { return _x / len(); }
double Point::cos() const { return _y / len(); }


/**
 * Print as e.g. "(1.004571e+06, -3.00000e+01)".
 */
std::ostream& Point::put(std::ostream& os) const
{
    char buf[8+6];
    std::sprintf(buf, "%.6e", _x);
    os << '(' << buf << ", ";
    std::sprintf(buf, "%.6e", _y);
    return os << buf << ')';
}

std::ostream& operator<< (std::ostream& os, const Point& val)
{
    return val.put(os);
}
