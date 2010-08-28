/*
 * $Id: transform.cc,v 1.4 2010-08-28 08:17:46 grahn Exp $
 *
 * Copyright (c) 2003, 2010 Jörgen Grahn <grahn+src@snipabacken.se>
 * All rights reserved.
 */
#include "transform.h"

namespace {

    /* sin(a-b) = sin(a)cos(b) + cos(a)sin(b)
     * cos(a-b) = cos(a)cos(b) + sin(a)sin(b)
     */
    inline double sin_sub(double sa, double ca, double sb, double cb)
    {
	return sa*cb + ca*sb;
    }

    inline double cos_sub(double sa, double ca, double sb, double cb)
    {
	return ca*cb + sa*sb;
    }

    inline SinCos sincos_sub(const SinCos& a, const SinCos& b)
    {
	return SinCos(a.sin*b.cos + a.cos*b.sin,
		      a.cos*b.cos + a.sin*b.sin);
    }
}


Transform::Transform(const RT90& src_a, const Pixel& dst_a,
		     const RT90& src_b, const Pixel& dst_b)
    : A(0), B(0), C(0), D(0), E(0), F(0)
{
    const Point sv = src_b.p - src_a.p;
    const Point dv = dst_b.p - dst_a.p;

    /* The angle, or rotation, between src and dst, expressed as
     * (sin v, cos v) rather than simply v because ...
     * well, I guess I mostly want to avoid trigonometry to show off.
     */
    const SinCos v = sincos_sub(dv.sincos(), sv.sincos());

}


Pixel& Transform::operator() (const RT90& src) const
{
    const Point s = src.p;
    return Pixel(A*s.x + B*s.y + C,
		 D*s.x + E*s.y + F);
}
