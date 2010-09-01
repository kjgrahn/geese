/*
 * $Id: transform.cc,v 1.9 2010-09-01 20:27:00 grahn Exp $
 *
 * Copyright (c) 2003, 2010 Jörgen Grahn <grahn+src@snipabacken.se>
 * All rights reserved.
 */
#include "transform.h"

#include <cstdio>
#include <iostream>

namespace {

    /* sin(a-b) = sin(a)cos(b) - cos(a)sin(b)
     * cos(a-b) = cos(a)cos(b) + sin(a)sin(b)
     */
    inline SinCos sincos_sub(const SinCos& a, const SinCos& b)
    {
	return SinCos(a.sin*b.cos - a.cos*b.sin,
		      a.cos*b.cos + a.sin*b.sin);
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
    std::sprintf(buf, "%.1f %.1f", val.p.x, -val.p.y);
    return os << buf;
}



Transform::Transform(const RT90& src_a, const Pixel& dst_a,
		     const RT90& src_b, const Pixel& dst_b)
    : A(0), B(0), C(0), D(0), E(0), F(0)
{
    const Point sv = src_b.p - src_a.p;
    const Point dv = dst_b.p - dst_a.p;
    //std::cerr << "src " << sv.sincos() << '\n';
    //std::cerr << "dst " << dvs << '\n';

    /* The angle, or rotation, between src and dst, expressed as
     * (sin v, cos v).
     */
    const SinCos rotation = sincos_sub(dv.sincos(), sv.sincos());
    //std::cerr << "rotation " << rotation << '\n';

    /* Likewise, the scaling.
     */
    const double scaling = dv.len()/sv.len();

    /* The translation can be calculated from e.g. pair A:
     * T = dst - SR src
     */
    const Point transl = dst_a.p - scale(scaling, rotate(rotation, src_a.p));

    A = scaling * rotation.cos;
    E = A;
    D = scaling * rotation.sin;
    B = -D;
    C = transl.x;
    F = transl.y;
}


Pixel Transform::operator() (const RT90& src) const
{
    const Point s = src.p;
    return Pixel(A*s.x + B*s.y + C,
		 D*s.x + E*s.y + F);
}
