/*
 * $Id: transform.cc,v 1.1 2003-01-05 23:19:02 grahn Exp $
 *
 * transform.cc
 *
 * Copyright (c) 2003 Jörgen Grahn <jgrahn@algonet.se>
 * All rights reserved.
 */

#include "transform.h"


Vector Scale::operator() (const Vector& v) const
{
    return Vector(v.x() * _s, v.y() * _s);
}


Vector Transpose::operator() (const Vector& v) const
{
    return Vector(v.x() + _v.x(), v.y() + _v.y());
}


Vector Rotate::operator() (const Vector& v) const
{
    return Vector(_cos * v.x() - _sin * v.y(),
		  _sin * v.x() + _cos * v.y());
}


/**
 * Create a plane transform given two distinct
 * points A and B in the source coordinate system (1),
 * and their corresponding points in the target
 * coordinate system (2).
 *
 */
PlaneTransform::PlaneTransform(const Vector& a1, const Vector& a2,
			       const Vector& b1, const Vector& b2)
    : _t1(Vector() - a1),
      _r(angle(a2, b2) - angle(a1, b1)),
      _t2(a2),
      _s(distance(a2, b2) / distance(a1, b1))
{
}


Vector PlaneTransform::operator() (const Vector& v) const
{
    /*
     * t1 - transpose; A1 to origo
     * r  - rotate; <A1, B1> parallell to <A2, B2>
     * s  - scale; |A1-B1| == |A2-B |
     * t2 - transpose; origo to A2
     *
     */

    return _t2(_s(_r(_t1(v))));
}
