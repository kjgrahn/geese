/*
 * $Id: transform.cc,v 1.17 2010-09-12 12:59:14 grahn Exp $
 *
 * Copyright (c) 2003, 2010 Jörgen Grahn <grahn+src@snipabacken.se>
 * All rights reserved.
 */
#include "transform.h"

#include <cstdio>
#include <iostream>
#include <cmath>

namespace {

    /* sin(a-b) = sin(a)cos(b) - cos(a)sin(b)
     * cos(a-b) = cos(a)cos(b) + sin(a)sin(b)
     */
    inline SinCos sincos_sub(const SinCos& a, const SinCos& b)
    {
	return SinCos(a.sin*b.cos - a.cos*b.sin,
		      a.cos*b.cos + a.sin*b.sin);
    }

    inline SinCos mirror(const SinCos a)
    {
	return SinCos(-a.sin, a.cos);
    }

    inline Point mirror(const Point a)
    {
	return Point(a.x, -a.y);
    }

    inline Point scale(double s, const Point& p)
    {
	return Point(s*p.x, s*p.y);
    }

    inline Point rotate(const SinCos& r, const Point& p)
    {
	return Point(r.cos * p.x - r.sin * p.y,
		     r.sin * p.x + r.cos * p.y);
    }
}


/**
 * Print as e.g. "6442400.0 1362217.0".
 */
std::ostream& operator<<  (std::ostream& os, const RT90& val)
{
    char buf[30];
    std::sprintf(buf, "%.1f %.1f", val.p.y, val.p.x);
    return os << buf;
}

/**
 * Print as e.g. "800 600".
 */
std::ostream& operator<<  (std::ostream& os, const Pixel& val)
{
    char buf[30];
    std::sprintf(buf, "%.1f %.1f", val.p.x, val.p.y);
    return os << buf;
}


/**
 * The Transform for which
 *   T(src_a) = dst_a
 *   T(src_b) = dst_b.
 */
Transform::Transform(const RT90& src_a, const Pixel& dst_a,
		     const RT90& src_b, const Pixel& dst_b)
    : in_(src_a.p, dst_a.p,
	  src_b.p, dst_b.p),
      out_(dst_a.p, src_a.p,
	   dst_b.p, src_b.p)
{}


Transform::Srrt::Srrt(const Point& src_a, const Point& dst_a,
		      const Point& src_b, const Point& dst_b)
{
    const Point sv = src_b - src_a;
    const Point dv = dst_b - dst_a;

    /* The angle, or rotation, between src and dst, expressed as
     * (sin v, cos v).
     */
    const SinCos rotation = sincos_sub(sv.sincos(), mirror(dv.sincos()));

    /* Likewise, the scaling.
     */
    const double scaling = dv.len()/sv.len();

    /* The translation can be calculated from e.g. pair A:
     * T = dst - SR src
     * and some mirroring I don't quite understand
     */
    const Point transl = dst_a - ::scale(scaling, rotate(rotation, mirror(src_a)));

    A = scaling * rotation.cos;
    E = -A;
    D = scaling * rotation.sin;
    B = D;
    C = transl.x;
    F = transl.y;
}


/**
 * The scaling part of the transform, in the direction Pixel->RT90.
 * For example, scale()==5 means "each pixel is 5m".
 */
double Transform::scale() const
{
    /* sin²+cos² = 1
     * A² + D² = scaling²(sin²+cos²)
     * scaling = sqrt(A² + D²)
     * ... and we're really looking for its inverse
     */
    const double A = in_.A;
    const double D = in_.D;
    return 1/std::sqrt(A*A+D*D);
}


/**
 * The rotation part of the transform, in degrees, in the direction
 * Pixel->RT90.  For example, rotation()==45 means "if you rotate the
 * map 45° counter-clockwise, the grids are aligned".
 */
double Transform::rotation() const
{
    const double A = in_.A;
    const double D = in_.D;
    return std::atan2(D, A) * 180 / M_PI;
}


std::ostream& Transform::put(std::ostream& os) const
{
    return out_.put(os);
}


Transform::Srrt::Srrt()
    : A(1), B(0),  C(0),
      D(0), E(-1), F(0)
{}


Point Transform::Srrt::operator() (const Point& s) const
{
    return Point(A*s.x + B*s.y + C,
		 D*s.x + E*s.y + F);
}


std::ostream& Transform::Srrt::put(std::ostream& os) const
{
    char buf[50];
    std::sprintf(buf, "%+.3e  %+.3e  %+.3e", A, B, C);
    os << "A B C [" << buf << "]\n";
    std::sprintf(buf, "%+.3e  %+.3e  %+.3e", D, E, F);
    os << "D E F [" << buf << "]";
    return os;
}
