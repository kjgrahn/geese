/* -*- c++ -*-
 *
 * $Id: vector.h,v 1.1 2003-01-02 20:30:23 grahn Exp $
 *
 * vector.h
 *
 * Copyright (c) 2003 Jörgen Grahn <jgrahn@algonet.se>
 * All rights reserved.
 */
#ifndef VECTOR_H
#define VECTOR_H

#include <iosfwd>

/**
 * A two-dimentional vector; a point in a plane.
 *
 */
class Vector
{
public:
    Vector(double x = 0.0, double y = 0.0) : _x(x), _y(y) {}

    double x() const {return _x;}
    double y() const {return _y;}

    double length() const;
    double angle() const;

private:
    double _x;
    double _y;
};


Vector operator+ (const Vector& a, const Vector& b);
Vector operator- (const Vector& a, const Vector& b);

double angle(const Vector& a, const Vector& b) {return (b-a).angle();}

double distance(const Vector& a, const Vector& b) {return (b-a).length();}

std::ostream& operator<< (std::ostream& o, const Vector& v);

#endif
