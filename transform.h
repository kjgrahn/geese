/* -*- c++ -*-
 *
 * $Id: transform.h,v 1.4 2004-06-16 19:02:33 grahn Exp $
 *
 * transform.h
 *
 * Copyright (c) 2003, 2004 Jörgen Grahn <jgrahn@algonet.se>
 * All rights reserved.
 */
#ifndef TRANSFORM_H
#define TRANSFORM_H

#include <cmath>
#include "vector.h"

/**
 * A vector transform functor.
 *
 */
class Transform
{
public:
    virtual Vector operator() (const Vector& v) const  = 0;
};


/**
 * A plane scaling; f(v) = Sv,  where S =
 *
 * [ s   0 ]
 * [       ]
 * [ 0   s ]
 *
 */
class Scale: public Transform
{
public:
    explicit Scale(double s = 1.0) : _s(s) {}
    virtual Vector operator() (const Vector& v) const;

private:
    const double _s;
};


/**
 * A plane transposition; f(v) = v + v'
 *
 */
class Transpose: public Transform
{
public:
    Transpose() : _v(0.0, 0.0) {}
    explicit Transpose(const Vector& v) : _v(v) {}
    virtual Vector operator() (const Vector& v) const;

private:
    const Vector _v;
};


/**
 * A plane rotation around origo; f(v) = Rv,  where R =
 *
 * [ cos -sin ]
 * [          ]
 * [ sin  cos ]
 *
 */
class Rotate: public Transform
{
public:
    explicit Rotate(double a = 0.0)
	: _sin(std::sin(a)),
	  _cos(std::cos(a)) {}
    virtual Vector operator() (const Vector& v) const;

private:
    const double _sin;
    const double _cos;
};


/**
 * A transform between two planar coordinate systems,
 * where one is a scaled, (slightly) rotated
 * and transposed copy of the other.
 *
 * Example: pixel coordinates in a scanned map,
 * and a coordinate system across the terrain.
 *
 */
class PlaneTransform: public Transform
{
public:
    PlaneTransform(const Vector& a1, const Vector& b1,
		   const Vector& a2, const Vector& b2);

    virtual Vector operator() (const Vector& v) const;

private:
    const Transpose _t1;
    const Rotate _r;
    const Transpose _t2;
    const Scale _s;
};


/**
 * A lame transform for coordinate systems with origo
 * in the upper-left corner -- e.g. bitmaps.
 *
 *  -----o---->------    -----------------
 *   ^   |                ^
 *  h|   |               h|   ^
 *   |   v                |   |
 *   v                    v   |
 *  -----------------    -----o---->------
 *
 */
class BitmapTransform: public Transform
{
public:
    explicit BitmapTransform(double h) : _h(h) {}

    virtual Vector operator() (const Vector& v) const {
	return Vector(v.x(), _h - v.y());
    }

private:
    const double _h;
};

#endif
