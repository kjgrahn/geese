/* $Id: geese.cc,v 1.4 2003-01-05 23:11:06 grahn Exp $
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
    Scale(double s = 1.0) : _s(s) {}
    virtual Vector operator() (const Vector& v) const;

private:
    double _s;
};

Vector Scale::operator() (const Vector& v) const
{
    return Vector(v.x() * _s, v.y() * _s);
}


/**
 * A plane transposition; f(v) = v + v'
 *
 */
class Transpose: public Transform
{
public:
    Transpose() : _v(0.0, 0.0) {}
    Transpose(const Vector& v) : _v(v) {}
    virtual Vector operator() (const Vector& v) const;

private:
    Vector _v;
};

Vector Transpose::operator() (const Vector& v) const
{
    return Vector(v.x() + _v.x(), v.y() + _v.y());
}


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
    Rotate(double a = 0.0)
	: _sin(std::sin(a)),
	  _cos(std::cos(a)) {}
    virtual Vector operator() (const Vector& v) const;

private:
    double _sin;
    double _cos;
};

Vector Rotate::operator() (const Vector& v) const
{
    return Vector(_cos * v.x() - _sin * v.y(),
		  _sin * v.x() + _cos * v.y());
}


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
    Transpose _t1;
    Rotate _r;
    Transpose _t2;
    Scale _s;
};


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


int main()
{
    using std::cout;
    using std::endl;

    const Vector a2(1, 3);
    const Vector b2(0, 1);

    PlaneTransform tt(Vector(0,0), a2,
		      Vector(1,0), b2);

    cout << angle(Vector(0,0), Vector(1,0)) << "; "
	 << angle(a2, b2) << endl;

    cout << Vector(0,0) << endl;
    cout << Vector(1,0) << endl;
    cout << a2 << endl;
    cout << b2 << endl;
    cout << endl;

    cout << tt(Vector(0,0)) << endl;
    cout << tt(Vector(1,0)) << endl;
    cout << tt(Vector(0,1)) << endl;
    cout << tt(Vector(1,1)) << endl;
    cout << tt(Vector(1.2,1.2)) << endl;

    return 0;
}
