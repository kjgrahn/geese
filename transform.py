#!/usr/bin/env python
# -*- coding: iso-8859-1 -*-
#
# $Id: transform.py,v 1.8 2004-06-27 20:46:21 grahn Exp $
#
# Copyright (c) 2004 Jörgen Grahn <jgrahn@algonet.se>
# All rights reserved.
#
"""Two-dimensional vector transform functors:
scaling, transposition and rotation.

Or put differently, functors for distorting
coordinate systems.
"""

import math
from geese import vector

class scale:
    """A plane scaling; f(v) = Sv,  where

        [ s   0 ]
    S = [       ]
        [ 0   s ]
    """
    def __init__(self, s = 1):
        self._s = s
    def __call__(self, v):
        vx, vy = v
        return self._s * vx, self._s * vy

class transpose:
    """A plane transposition; f(v) = v + v'"""
    def __init__(self, v = (0,0)):
        self.v = v
    def __call__(self, v):
        return vector.add(v, self.v)

class rotate:
    """A plane rotation around origo; f(v) = Rv,  where
    
        [ cos -sin ]
    R = [          ]
        [ sin  cos ]
    """
    def __init__(self, a = 0):
        self.sin = math.sin(a)
        self.cos = math.cos(a)
    def __call__(self, v):
        sin = self.sin
        cos = self.cos
        x, y = v
        return (cos*x - sin*y,
                sin*x + cos*y)

if __name__ == "__main__":
    import unittest

    class test(unittest.TestCase):
        if not hasattr(unittest.TestCase, 'assertAlmostEqual'):
            # no assertAlmostEqual() in Python 2.2
            def assertAlmostEqual(self, first, second):
                if first==second: return
                if not abs(first*0.9999) < abs(second) < abs(first*1.0001):
                    raise self.failureException, \
                          '%s != %s' % (`first`, `second`)
        vectors = ((70.771, 12.361),
                   (43.075, 67.525),
                   (71.164, 25.736),
                   (1.7607, .034326),
                   (.061730, .062715),
                   (-70202.0, -36474),
                   (-12503, -.15746),
                   (-.00433, -22.440),
                   (-41.635, -7.7276),
                   (-7.2240, -5.1554),
                   (-5.4665, -1.1266),
                   (-3.2707, -2.3626),
                   (-77251.0, -15076.0),
                   (-5.5053, .17073),
                   (4.7345, 3.4166),
                   (-70.27, -45.72),
                   (.64631, .22554),
                   (.06504, .41170),
                   (.65420, 1.2410),
                   (3.6106, 50470.0),
                   (73054.0, -63445.0),
                   (-611.3, -15.12),
                   (3.2535, 4.0270),
                   (40.203, 31.350),
                   (-43046.0, -15.463),
                   (-0.5724, -5.2021),
                   (-4.2741, 3.4423),
                   (5.5517, 4.3667),
                   (1.2726, 1.5472),
                   (1.1742, .010456),
                   (.031014, .036227),
                   (.060737, .037544))
        def testScale(self):
            origo = (0,0)
            for s in xrange(1, 40):
                s /= 13.0
                sf = scale(s)
                for v in self.vectors:
                    v2 = sf(v)
                    self.assertAlmostEqual(vector.angle(origo, v),
                                           vector.angle(origo, v2))
                    self.assertAlmostEqual(s*vector.length(v),vector.length(v2))
        def testTranspose(self):
            tf = transpose((-0.14, 782.01))
            origo = (0,0)
            origot = tf(origo)
            for v in self.vectors:
                v2 = tf(v)
                self.assertAlmostEqual(vector.angle(origo, v),
                                       vector.angle(origot, v2))
                self.assertAlmostEqual(vector.distance(origo, v),
                                       vector.distance(origot, v2))
        def testRotate(self):
            for angle in xrange(-314, 314):
                angle /= 100.0
                rf = rotate(angle)
                for v in self.vectors:
                    v2 = rf(v)
                    self.assertAlmostEqual(vector.length(v), vector.length(v2))

    unittest.main()
