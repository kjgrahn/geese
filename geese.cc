/* $Id: geese.cc,v 1.1 2003-01-02 20:30:23 grahn Exp $
 *
 * geese.cc
 *
 * Copyright (c) 2003 Jörgen Grahn <jgrahn@algonet.se>
 * All rights reserved.
 */

#include <iostream>

#include "vector.h"

/**
 * A vector transform functor.
 *
 */
class Transform
{
public:
    virtual Vector operator() (const Vector& v) const = 0;
};


/**
 * A plane scaling, as a matrix
 * [[s 0] [0 s]] .
 *
 */
class Scale: public Transform
{
public:
    Scale(double s) : _s(s) {}

private:
    double _s;
};


/**
 * A plane transposition.
 *
 */
class Transpose: public Transform
{
public:
    Transpose(const Vector& v) : _v(v) {}

private:
    Vector _v;
};


/**
 * A plane rotation around origo.
 *
 */
class Rotate: public Transform
{
public:
    Rotate(double a) : _a(a) {}

private:
    double _a;
};


/**
 * A transform between two planar coordinate systems,
 * where one is a scaled, (slightly) rotated
 * and transposed copy of the other.
 *
 * Example: pixel coordinates in a scanned map,
 * and a coordinate system in the terrain.
 *
 */
class PlaneTransform
{
public:
    PlaneTransform(const Vector& a1, const Vector& b1,
		   const Vector& a2, const Vector& b2);

private:
};
