#!/usr/bin/env python
# -*- coding: iso-8859-1 -*-
#
# $Id: vector.py,v 1.6 2004-08-07 15:58:10 grahn Exp $
#
# Copyright (c) 2004 Jörgen Grahn <jgrahn@algonet.se>
# All rights reserved.
#
"""Two-dimensional vector operations, with vectors represented as
2-tuples of numbers (for simplicity, there is no Vector class per se).
"""

import math

origo = (0,0)

def length(v):
    "Length of a vector."
    x, y = map(float, v)
    return math.sqrt(x*x + y*y)
def add(v1, v2):
    "Add two vectors."
    x1, y1 = v1
    x2, y2 = v2
    return x1+x2, y1+y2
def sub(v1, v2):
    "Subtract two vectors."
    x1, y1 = v1
    x2, y2 = v2
    return x1-x2, y1-y2
def angle(v1, v2):
    """The angle between two points (vectors),
    i.e. from v1 to v2.
    """
    x1, y1 = v1
    x2, y2 = v2
    return math.atan2(y2-y1, x2-x1)
def distance2(v1, v2):
    "The square of the distance between two points (vectors)."
    x, y = sub(v2, v1)
    return x*float(x) + y*float(y)
def distance(v1, v2):
    "The distance between two points (vectors)."
    return math.sqrt(distance2(v1, v2))

if __name__ == "__main__":
    import unittest

    class test(unittest.TestCase):
        def testLength(self):
            self.assertEqual(length(origo), 0)
            for y in range(-100, 1000):
                self.assertEqual(length((0, y)), math.fabs(y))
            for x in range(-100, 1000):
                self.assertEqual(length((x, 0)), math.fabs(x))
        def testAngle(self):
            for i in range(1, 1000):
                i /= 10.0
                self.assertEqual(angle(origo, (i, 0)), 0)
                self.assertEqual(angle(origo, (i, i)), math.pi/4)
                self.assertEqual(angle(origo, (0, i)), math.pi/2)
                self.assertEqual(angle(origo, (-i, i)), math.pi*3/4)
                self.assertEqual(angle(origo, (-i, 0)), math.pi)
        def testAngle2(self):
            self.assertEqual(angle((1,1), (2,1)), 0)
            self.assertEqual(angle((2,1), (1,1)), math.pi)
            self.assertEqual(angle((1,1), (1,2)), math.pi/2)

    unittest.main()
