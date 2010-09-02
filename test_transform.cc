/**
 * $Id: test_transform.cc,v 1.10 2010-09-02 21:15:22 grahn Exp $
 *
 * Copyright (c) 2010 Jörgen Grahn
 * All rights reserved.
 *
 */
#include "transform.h"

#include <testicle.h>

namespace {

    /**
     * a < val < b
     */
    void assert_in(double a, double val, double b)
    {
	testicle::assert_lt(a, val);
	testicle::assert_lt(val, b);
    }

    /**
     * val is in a ± epsilon
     */
    void assert_near(double val, double a, double epsilon)
    {
	assert_in(a-epsilon, val, a+epsilon);
    }

    /**
     * T(a) == b, plus a fuzz factor.
     */
    void assert_near(const RT90& a, const Pixel& b, double epsilon,
		     const Transform& T)
    {
	assert_near(distance(T(a), b), 0, epsilon);
    }

    /**
     * T scales the distance between any two points in 'src[]'
     * by the same factor.
     */
    void assert_uniform(const RT90* const src, const int N,
			const Transform& T)
    {
	const double s = distance(T(RT90(0,0)), T(RT90(0,1)));
	for(int m=0; m<N; ++m) for(int n=0; n<N; ++n) {
	    const RT90 a = src[m];
	    const RT90 b = src[n];
	    assert_near(distance(T(a), T(b)),
			s * distance(a, b),
			1e-6);
	}
    }
}

namespace Aaa {

    /* Aa@ in the 'fixed' font, with RT90(0, 0)
     * in the left leg of the 'A'.
     * The bounding box is RT90(0, 0) and RT90(8, 16):
     *
     * 8,0  8,16
     * 0,0  0,16
     */
    static const RT90 src[] = {
	RT90( 0,  0), RT90( 1,  0), RT90( 2,  0),
	RT90( 3,  0), RT90( 4,  0), RT90( 5,  0),
	RT90( 6,  0), RT90( 7,  1), RT90( 8,  2),
	RT90( 7,  3), RT90( 6,  4), RT90( 5,  4),
	RT90( 4,  4), RT90( 3,  4), RT90( 3,  3),
	RT90( 3,  2), RT90( 3,  1), RT90( 2,  4),
	RT90( 1,  4), RT90( 0,  4),
    
	RT90( 5,  7), RT90( 5,  8), RT90( 5,  9),
	RT90( 4, 10), RT90( 3, 10), RT90( 3,  9),
	RT90( 3,  8), RT90( 3,  7), RT90( 2,  6),
	RT90( 1,  6), RT90( 0,  7), RT90( 0,  8),
	RT90( 1,  9), RT90( 2, 10), RT90( 1, 10),
	RT90( 0, 10),
    
	RT90( 0, 16), RT90( 0, 15), RT90( 0, 14),
	RT90( 0, 13), RT90( 1, 12), RT90( 2, 12),
	RT90( 3, 12), RT90( 4, 12), RT90( 5, 12),
	RT90( 6, 12), RT90( 7, 12), RT90( 8, 13),
	RT90( 8, 14), RT90( 8, 15), RT90( 7, 16),
	RT90( 6, 16), RT90( 5, 16), RT90( 4, 16),
	RT90( 3, 16), RT90( 2, 15), RT90( 2, 14),
	RT90( 3, 14), RT90( 4, 14), RT90( 5, 15)
    };
    static const int N = sizeof(src)/sizeof(src[0]);

    void print(std::ostream& os, const Transform& T)
    {
	os << '\n';
	for(int n=0; n<N; ++n) {
	    Pixel p = T(src[n]);
	    os << p << '\n';
	}
    }
}


namespace transform {

    void test_unrotated()
    {
	const Transform T(RT90(8, 0),  Pixel(0, 0),
			  RT90(0, 16), Pixel(16, 8));

#if 0
	std::cout << distance(T(RT90(8, 0)), Pixel(0, 0)) << '\n';
	std::cout << distance(T(RT90(0, 16)), Pixel(16, 8)) << '\n';
	std::cout << T(RT90(0, 16)) << '\n';
	std::cout << Pixel(16, 8) << '\n';
#endif
	assert_uniform(Aaa::src, Aaa::N, T);
	assert_near(RT90(8, 0), Pixel(0, 0), 1e-6, T);
	assert_near(RT90(0,16), Pixel(16,8), 1e-6, T);
    }

    void test_transposed()
    {
	const Transform T(RT90(8, 0),  Pixel(4, 10),
			  RT90(0, 16), Pixel(20,18));

	assert_uniform(Aaa::src, Aaa::N, T);
	assert_near(RT90(8, 0), Pixel(4, 10), 1e-6, T);
	assert_near(RT90(0,16), Pixel(20,18), 1e-6, T);
    }

    void test_blowup()
    {
	const Transform T(RT90(0, 0), Pixel(0,20),
			  RT90(0,16), Pixel(30,20));

	assert_uniform(Aaa::src, Aaa::N, T);
	assert_near(RT90(0, 0),  Pixel( 0,20), 1e-6, T);
	assert_near(RT90(0, 16), Pixel(30,20), 1e-6, T);
    }

    void test_slant()
    {
	const Transform T(RT90(0, 0), Pixel( 0,20),
			  RT90(0,16), Pixel(30,30));

	assert_uniform(Aaa::src, Aaa::N, T);
	assert_near(RT90(0, 0), Pixel( 0,20), 1e-6, T);
	assert_near(RT90(0,16), Pixel(30,30), 1e-6, T);
    }
}

#if 1
namespace fun {

    void test_unrotated()
    {
	const Transform T(RT90(8, 0),  Pixel(0, 0),
			  RT90(0, 16), Pixel(16, 8));
	std::cout << '\n' << T << '\n';
	Aaa::print(std::cout, T);
    }

    void test_transposed()
    {
	const Transform T(RT90(8, 0),  Pixel(4, 10),
			  RT90(0, 16), Pixel(20,18));
	std::cout << '\n' << T << '\n';
	Aaa::print(std::cout, T);
    }

    void test_down()
    {
	const Transform T(RT90(0, 0),  Pixel(10,20),
			  RT90(0, 16), Pixel(10,36));
	std::cout << '\n' << T << '\n';
	Aaa::print(std::cout, T);
    }

    void test_blowup()
    {
	const Transform T(RT90(0, 0),  Pixel(0,20),
			  RT90(0, 16), Pixel(30,20));
	std::cout << '\n' << T << '\n';
	Aaa::print(std::cout, T);
    }

    void test_slant()
    {
	const Transform T(RT90(0, 0),  Pixel(0,20),
			  RT90(0, 16), Pixel(30,30));
	std::cout << '\n' << T << '\n';
	Aaa::print(std::cout, T);
    }
}
#endif

#if 0
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
#endif
