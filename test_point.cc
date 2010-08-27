/**
 * $Id: test_point.cc,v 1.4 2010-08-27 20:35:16 grahn Exp $
 *
 * Copyright (c) 2010 Jörgen Grahn
 * All rights reserved.
 *
 */
#include "point.h"

#include <testicle.h>

namespace {

    void assert_in(double a, double val, double b)
    {
	testicle::assert_lt(a, val);
	testicle::assert_lt(val, b);
    }

    void assert_near(double a, double val)
    {
	const double epsilon = 1e-6;
	testicle::assert_lt(a-epsilon, val);
	testicle::assert_lt(val, a+epsilon);
    }

}

namespace point {

    const Point  e( 1,  0);
    const Point ne( 1,  1);
    const Point  n( 0,  1);
    const Point nw(-1,  1);
    const Point  w(-1,  0);
    const Point sw(-1, -1);
    const Point  s( 0, -1);
    const Point se( 1, -1);

    void test_length()
    {
	assert_near(1, e.len());
	assert_near(1, n.len());
	assert_near(1, w.len());
	assert_near(1, s.len());

	assert_near(1.414214, ne.len());
	assert_near(1.414214, nw.len());
	assert_near(1.414214, sw.len());
	assert_near(1.414214, se.len());

	assert_near(1.118034, Point(1, .5).len());

	assert_near(1000, Point(0, 1000).len());

	assert_near(1e8, Point(0, 1e8).len());
    }

    void test_sin()
    {
	assert_near( 0, e.sin());
	assert_near( 1, n.sin());
	assert_near( 0, w.sin());
	assert_near(-1, s.sin());

	assert_near(0.70710678, ne.sin());
	assert_near(0.70710678, nw.sin());
	assert_near(-.70710678, sw.sin());
	assert_near(-.70710678, se.sin());

	assert_near(0.70710678, Point(1e6, 1e6).sin());
    }

    void test_cos()
    {
	assert_near( 1, e.cos());
	assert_near( 0, n.cos());
	assert_near(-1, w.cos());
	assert_near( 0, s.cos());

	assert_near(0.70710678, ne.cos());
	assert_near(-.70710678, nw.cos());
	assert_near(-.70710678, sw.cos());
	assert_near(0.70710678, se.cos());

	assert_near(0.70710678, Point(1e6, 1e6).cos());
    }

    void test_distance()
    {
	const Point a(10, 10);
	const Point b(13, 14);
	assert_near(5, distance(a, b));
    }

}
