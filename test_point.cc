/**
 * $Id: test_point.cc,v 1.1 2010-08-25 21:37:40 grahn Exp $
 *
 * Copyright (c) 2010 Jörgen Grahn
 * All rights reserved.
 *
 */
#include <point.h>

#include <testicle.h>

namespace {

    void assert_in(double a, double val, double b)
    {
	testicle::assert_lt(a, val);
	testicle::assert_lt(val, b);
    }

    void assert_near(double a, double val)
    {
	testicle::assert_lt(a*0.9999, val);
	testicle::assert_lt(val, a*1.0001);
    }

}

namespace point {

    void test_length()
    {
	assert_near(1, Point(1, 0).len());
	assert_near(1, Point(0, 1).len());

	assert_near(1.414214, Point(1, 1).len());
	assert_near(1.118034, Point(1, .5).len());

	assert_near(1000, Point(0, 1000).len());

	assert_near(1e8, Point(0, 1e8).len());
    }

    void test_sin()
    {
	assert_near(0, Point(1, 0).sin());
	assert_near(1, Point(1, 0).cos());

	assert_near(1, Point(0, 1).sin());
	assert_near(0, Point(0, 1).cos());

	assert_near(0.70710678, Point(1, 1).sin());
	assert_near(0.70710678, Point(1, 1).cos());

	assert_near(0.70710678, Point(1e6, 1e6).sin());
	assert_near(0.70710678, Point(1e6, 1e6).cos());
    }

}
