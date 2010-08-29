/**
 * $Id: test_transform.cc,v 1.5 2010-08-29 22:52:43 grahn Exp $
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

    void near(const Pixel& a, const Pixel& b)
    {
	std::cout << a.p.x << ' ' <<  a.p.y << '\n';
	//testicle::assert_lt(distance(a, b), 1e-6);
    }
}

namespace transform {

    /*
     *  n             0+-----> x
     *  |	     10|øxx
     * 5|øxx         20|x
     * 4|x	     30|xxx
     * 3|xxx         40|x
     * 2|x	     50|x ø
     * 1|x ø           |
     * 0+-----> e      y
     *  0123
     */
    void assert_f(const Transform& T)
    {
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
    }

    void test_upright()
    {
	const Transform T(RT90(5, 1), Pixel(10, 10),
			  RT90(1, 3), Pixel(30, 50));
	assert_f(T);
    }

    void test_right()
    {
	const Transform T(RT90(5, 1), Pixel(50, 50),
			  RT90(1, 3), Pixel(10, 70));
	assert_f(T);
    }

    void test_down()
    {
	const Transform T(RT90(5, 1), Pixel(10, 90),
			  RT90(1, 3), Pixel(-10, 50));
	assert_f(T);
    }

    void test_left()
    {
	const Transform T(RT90(5, 1), Pixel(-30, 50),
			  RT90(1, 3), Pixel( 10, 30));
	assert_f(T);
    }

    void test_ugly()
    {
	const Transform T(RT90(5, 1), Pixel(50, 0),
			  RT90(1, 1), Pixel(0, 100));
	assert_f(T);
    }

    void test_upright_world()
    {
	const Transform T(10,  0,  0,
			   0, 10,-60);
	assert_f(T);
    }

    void notest_transposed()
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
    }

}
