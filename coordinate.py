#!/usr/bin/env python
# -*- coding: iso-8859-1 -*-
#
# $Id: coordinate.py,v 1.1 2004-06-17 08:37:39 grahn Exp $
#
# Copyright (c) 2004 J�rgen Grahn <jgrahn@algonet.se>
# All rights reserved.
#
"""Translation between two coordinate systems, where
one is a (uniformly) scaled, rotated and transposed
version of the other.

Intended to provide a mapping between coordinates in the swedish
Rikets N�t projection (which appears as a square grid with origo to
the south-west of Sweden) on many national high-resolution maps, and
pixel coordinates in bitmap images generated by scanning a part of
such a map. Obviously, such scanned images are scaled and translated
relative to the Rikets N�t, and if the scan isn't perfect they tend to
be slightly rotated as well.

Bitmap images are, for the purposes of this module, assumed to have a
lower-left corner as origo.

As it turns out, only two points in the source and destination
coordinate systems are needed to determine the translation.
"""

class Transform:
    """A transformation between two coordinate systems,
    where one is a scaled, rotated and transposed version
    of the other.
    """
    def __init__(self, src_a, dst_a, src_b, dst_b):
        """Create a transformation from a source to a destination
        coordinate system, given the source and destination
        coordinates for two points A and B.

        Naturally, accurracy increases the further apart
        A and B are.
        """
        # for str)_ and inverse() only
        (self._src_a, self._dst_a,
         self._src_b, self._dst_b) = (src_a, dst_a, src_b, dst_b)
        

    def __call__(self, point_a):
        """Transform a point from coordinate system A to B."""
    def inverse(self):
        """Return the inverse of this transform, i.e.
        a transform from the destination to the source.
        """
        return Transform(self._dst_a, self._src_a,
                         self._dst_b, self._src_b)

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
        def testSlipsiken(self):
            bridge = (144808, 720870)
            fjelka = (144600, 721200)
            storjen = (145200, 720600)
            map1 = Transform(fjelka, (74, 118),
                             storjen, (430, 471))
            self.assertEqual(map1(bridge),
                             (197, 312))
            map2 = Transform(fjelka, (119, 124),
                             storjen, (437, 511))
            self.assertEqual(map2(bridge),
                             (221, 329))
            map3 = Transform(fjelka, (88, 168),
                             storjen, (483, 476))
            self.assertEqual(map3(bridge),
                             (233, 346))
            map4 = Transform(fjelka, (120, 190),
                             storjen, (689, 755))
            self.assertEqual(map4(bridge),
                             (316, 500))

    unittest.main()