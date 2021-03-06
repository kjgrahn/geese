/*
 * $Id: transform.cc,v 1.21 2010-09-18 20:35:48 grahn Exp $
 *
 * Copyright (c) 2003, 2010 J�rgen Grahn <grahn+src@snipabacken.se>
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
 * True iff the coordinate seems to be in Sweden.
 */
bool valid(const RT90& c)
{
    auto in_range = [] (unsigned a, double v, unsigned b) {
	return a < v && v < b;
    };

    return in_range(6100000, c.p.y, 7800000) &&
	   in_range(1200000, c.p.x, 1900000);
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
 * Print in reduced resolution, with rounding:
 * 6442400 1362217  1m
 * 644240 136222   10m
 * 64424 13622    100m
 * 6442 1362     1000m
 */
std::ostream& operator<< (std::ostream& os, const RT90::Format f)
{
    char buf[30];
    std::sprintf(buf, "%.0f %.0f",
		 f.p_.p.y/f.n_,
		 f.p_.p.x/f.n_);
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
 * The transform whose 'out' direction is
 *
 *   rt90 = AB pixel + C
 *          DE         F
 *
 * This corresponds to the six parameters in a 'world file'.
 *
 * XXX Strange things will happen if this isn't strictly
 * a scaling/rotation/mirroring/translation.
 */
Transform::Transform(double a, double b, double c,
		     double d, double e, double f)
    : out_(a, b, c,
	   d, e, f)
{
    /* XXX A bit of a cop-out to define one direction by
     * applying the other on a set of points, perhaps.
     */
    const Point p0(0, 0);
    const Point p1(1, 0);
    in_ = Srrt(out_(p0), p0,
	       out_(p1), p1);
}


/**
 * The scaling part of the transform, in the direction Pixel->RT90.
 * For example, scale()==5 means "each pixel is 5m".
 */
double Transform::scale() const
{
    /* sin�+cos� = 1
     * A� + D� = scaling�(sin�+cos�)
     * scaling = sqrt(A� + D�)
     * ... and we're really looking for its inverse
     */
    const double A = in_.A;
    const double D = in_.D;
    return 1/std::sqrt(A*A+D*D);
}


/**
 * The rotation part of the transform, in degrees, in the direction
 * Pixel->RT90.  For example, rotation()==45 means "if you rotate the
 * map 45� counter-clockwise, the grids are aligned".
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


Transform::Srrt::Srrt(double a, double b, double c,
		      double d, double e, double f)
    : A(a), B(b), C(c),
      D(d), E(e), F(f)
{}


Point Transform::Srrt::operator() (const Point& s) const
{
    return Point(A*s.x + B*s.y + C,
		 D*s.x + E*s.y + F);
}


std::ostream& Transform::Srrt::put(std::ostream& os) const
{
    char buf[50];
    std::sprintf(buf, "%+.3e  %+.3e  %+.1f", A, B, C);
    os << "A B C [" << buf << "]\n";
    std::sprintf(buf, "%+.3e  %+.3e  %+.1f", D, E, F);
    os << "D E F [" << buf << "]";
    return os;
}


/**
 * Print in the "world file" format, except without
 * MS-DOS line endings which seem common in the wild.
 *
 * Note that it's the map-to-world transform that gets
 * printed, and note the fairly careful choice of precision.
 */
std::ostream& Transform::worldfile(std::ostream& os) const
{
    char buf[80];
    const Srrt& s = out_;
    std::sprintf(buf,
		 "%.4f\n"
		 "%.4f\n"
		 "%.4f\n"
		 "%.4f\n"
		 "%.1f\n"
		 "%.1f",
		 s.A, s.D, s.B,
		 s.E, s.C, s.F);
    return os << buf;
}
