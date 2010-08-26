/**
 * $Id: test_transform.cc,v 1.1 2010-08-26 21:18:49 grahn Exp $
 *
 * Copyright (c) 2010 Jörgen Grahn
 * All rights reserved.
 *
 */
#include "transform.h"

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

namespace transform {

    /*
     *  n              +-----> x
     *  |	     10|øxx
     * 5|øxx         20|x
     * 4|x	     30|xxx
     * 3|xxx         40|x
     * 2|x	     50|xxø
     * 1|xxø           |
     *  +-----> e      y
     *  123
     */

    void test_simple()
    {
	const Transform T(RT90(5, 1), Pixel(10, 10),
			  RT90(1, 3), Pixel(30, 50));

	near(T(RT90(5,1)), Pixel(10, 10));
	near(T(RT90(4,1)), Pixel(10, 20));
	near(T(RT90(3,1)), Pixel(10, 30));
	near(T(RT90(2,1)), Pixel(10, 40));
	near(T(RT90(1,1)), Pixel(10, 50));

	near(T(RT90(5,1)), Pixel(10, 10));
	near(T(RT90(5,2)), Pixel(20, 10));
	near(T(RT90(5,3)), Pixel(30, 10));

	near(T(RT90(3,1)), Pixel(10, 30));
	near(T(RT90(3,2)), Pixel(20, 30));
	near(T(RT90(3,3)), Pixel(30, 30));

	near(T(RT90(1,1)), Pixel(10, 50));
	near(T(RT90(1,2)), Pixel(20, 50));
	near(T(RT90(1,3)), Pixel(30, 50));
    }

    void test_transposed()
    {
	const Transform T(RT90(6449005,1368001), Pixel(10, 10),
			  RT90(6449001,1368003), Pixel(30, 50));

	near(T(RT90(6449005,1368001)), Pixel(10, 10));
	near(T(RT90(6449004,1368001)), Pixel(10, 20));
	near(T(RT90(6449003,1368001)), Pixel(10, 30));
	near(T(RT90(6449002,1368001)), Pixel(10, 40));
	near(T(RT90(6449001,1368001)), Pixel(10, 50));

	near(T(RT90(6449005,1368001)), Pixel(10, 10));
	near(T(RT90(6449005,1368002)), Pixel(20, 10));
	near(T(RT90(6449005,1368003)), Pixel(30, 10));

	near(T(RT90(6449003,1368001)), Pixel(10, 30));
	near(T(RT90(6449003,1368002)), Pixel(20, 30));
	near(T(RT90(6449003,1368003)), Pixel(30, 30));

	near(T(RT90(6449001,1368001)), Pixel(10, 50));
	near(T(RT90(6449001,1368002)), Pixel(20, 50));
	near(T(RT90(6449001,1368003)), Pixel(30, 50));
    }

}
