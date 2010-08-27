/* -*- c++ -*-
 *
 * $Id: transform.h,v 1.7 2010-08-27 21:47:52 grahn Exp $
 *
 * transform.h
 *
 * Copyright (c) 2003, 2004, 2010 Jörgen Grahn <grahn+src@snipabacken.se>
 * All rights reserved.
 */
#ifndef GEESE_TRANSFORM_H
#define GEESE_TRANSFORM_H


class RT90 {
public:
    RT90(double north, double east);

    double distance(const RT90& other) const;

};

inline double distance(const RT90& a, const RT90& b) {
    return a.distance(b);
}


class Pixel {
public:
    Pixel(double x, double y) : x_(x), y_(y) {}

    double distance(const Pixel& other) const;

private:
    const double x_;
    const double y_;
};

inline double distance(const Pixel& a, const Pixel& b) {
    return a.distance(b);
}


class Transform {
public:
    Transform(const RT90& src_a, const Pixel& dst_a,
	      const RT90& src_b, const Pixel& dst_b);

    Pixel& operator() (const RT90& src) const;
};


#endif
