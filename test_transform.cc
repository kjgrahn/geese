/**
 * $Id: test_transform.cc,v 1.17 2010-09-12 06:19:37 grahn Exp $
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

namespace {

    void test(const RT90& sa, const Pixel& da,
	      const RT90& sb, const Pixel& db)
    {
	const Transform T1(sa, da, sb, db);
	const Transform T2(sb, db, sa, da);
 	assert_near(sa, da, 1e-6, T1);
 	assert_near(sa, da, 1e-6, T2);
 	assert_near(sb, db, 1e-6, T1);
 	assert_near(sb, db, 1e-6, T2);
 	assert_uniform(Aaa::src, Aaa::N, T1);
 	assert_uniform(Aaa::src, Aaa::N, T2);
    }
}

namespace freeform {

    void test_a()
    {
	for(int x=-100; x<100; x+=2) {
	    for(int y=-100; y<100; y+=2) {

		test(RT90(0, 0),  Pixel(11, 11),
		     RT90(0, 16), Pixel(x, y));
	    }
	}
    }

    void test_b()
    {
	for(int x=-100; x<100; x+=2) {
	    for(int y=-100; y<100; y+=2) {

		test(RT90(1, 1),  Pixel(11, 11),
		     RT90(1, 16), Pixel(x, y));
	    }
	}
    }
}

namespace transposition {

    void test_carpet()
    {
	for(int x=-100; x<100; x+=2) {
	    for(int y=-100; y<100; y+=2) {

		test(RT90(0, 0),  Pixel(x, y),
		     RT90(0, 16), Pixel(x+16, y));
	    }
	}
    }

    void test_carpet2()
    {
	for(int x=-100; x<100; ++x) {
	    for(int y=-100; y<100; ++y) {

		test(RT90(8, 0),  Pixel(x, y),
		     RT90(0, 16), Pixel(x+16, y+8));
	    }
	}
    }

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
}

namespace scaling {

    void test_blowup1()
    {
	const Transform T(RT90(0, 0), Pixel(0,16),
			  RT90(0,16), Pixel(32,16));

	assert_uniform(Aaa::src, Aaa::N, T);
	assert_near(RT90(0,0),  Pixel(0,16), 1e-6, T);
	assert_near(RT90(0,16), Pixel(32,16), 1e-6, T);
    }

    void test_blowup2()
    {
	const Transform T(RT90(0, 0), Pixel(0,16),
			  RT90(8, 0), Pixel(0, 0));

	assert_uniform(Aaa::src, Aaa::N, T);
	assert_near(RT90(0,0),  Pixel(0,16), 1e-6, T);
	assert_near(RT90(0,16), Pixel(32,16), 1e-6, T);
    }

    void test_blowup3()
    {
	const Transform T(RT90(0,16), Pixel(32,16),
			  RT90(8, 0), Pixel( 0, 0));

	assert_uniform(Aaa::src, Aaa::N, T);
	assert_near(RT90(0,0),  Pixel(0,16), 1e-6, T);
	assert_near(RT90(0,16), Pixel(32,16), 1e-6, T);
    }
}

namespace rotation {

    void test_e()
    {
	const Transform T(RT90(0, 0),  Pixel(0, 0),
			  RT90(0, 16), Pixel(16, 0));
	assert_uniform(Aaa::src, Aaa::N, T);
	assert_near(RT90(0, 0),  Pixel(0, 0), 1e-6, T);
	assert_near(RT90(0, 16), Pixel(16, 0), 1e-6, T);
    }

    void test_e2()
    {
	const Transform T(RT90(0, 16), Pixel(16, 0),
			  RT90(0, 0),  Pixel(0, 0));
	assert_uniform(Aaa::src, Aaa::N, T);
	assert_near(RT90(0, 0),  Pixel(0, 0), 1e-6, T);
	assert_near(RT90(0, 16), Pixel(16, 0), 1e-6, T);
    }

    void test_s()
    {
	const Transform T(RT90(0, 0),  Pixel(0, 0),
			  RT90(0, 16), Pixel(0, 16));
	assert_uniform(Aaa::src, Aaa::N, T);
	assert_near(RT90(0, 0),  Pixel(0, 0), 1e-6, T);
	assert_near(RT90(0, 16), Pixel(0, 16), 1e-6, T);
    }

    void test_w()
    {
	const Transform T(RT90(0, 0),  Pixel(0, 0),
			  RT90(0, 16), Pixel(-16, 0));
	assert_uniform(Aaa::src, Aaa::N, T);
	assert_near(RT90(0, 0),  Pixel(0, 0), 1e-6, T);
	assert_near(RT90(0, 16), Pixel(-16, 0), 1e-6, T);
    }

    void test_w2()
    {
	const Transform T(RT90(0, 16), Pixel(-16, 0),
			  RT90(0, 0),  Pixel(0, 0));
	assert_uniform(Aaa::src, Aaa::N, T);
	assert_near(RT90(0, 0),  Pixel(0, 0), 1e-6, T);
	assert_near(RT90(0, 16), Pixel(-16, 0), 1e-6, T);
    }

    void test_n()
    {
	const Transform T(RT90(0, 0),  Pixel(0, 0),
			  RT90(0, 16), Pixel(0, -16));
	assert_uniform(Aaa::src, Aaa::N, T);
	assert_near(RT90(0, 0),  Pixel(0, 0), 1e-6, T);
	assert_near(RT90(0, 16), Pixel(0, -16), 1e-6, T);
    }

    void test_se()
    {
	const Transform T(RT90(0, 0),  Pixel(0, 0),
			  RT90(0, 16), Pixel(11.31, 11.31));
	assert_uniform(Aaa::src, Aaa::N, T);
	assert_near(RT90(0, 0),  Pixel(0, 0), 1e-6, T);
	assert_near(RT90(0, 16), Pixel(11.31, 11.31), 1e-6, T);
    }

    void test_se2()
    {
	const Transform T(RT90(0, 16), Pixel(11.31, 11.31),
			  RT90(0, 0),  Pixel(0, 0));
	assert_uniform(Aaa::src, Aaa::N, T);
	assert_near(RT90(0, 0),  Pixel(0, 0), 1e-6, T);
	assert_near(RT90(0, 16), Pixel(11.31, 11.31), 1e-6, T);
    }
}
