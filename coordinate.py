#!/usr/bin/env python
# -*- coding: iso-8859-1 -*-
#
# $Id: coordinate.py,v 1.4 2004-06-17 10:18:35 grahn Exp $
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

import vector
import transform


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
        self._t1 = transform.transpose(vector.sub((0, 0), src_a))
        self._r = transform.rotate(vector.angle2(dst_a, dst_b) - \
                                   vector.angle2(src_a, src_b))
        self._t2 = transform.transpose(dst_a)
        self._s = transform.scale(vector.distance(dst_a, dst_b) / \
                                  vector.distance(src_a, src_b))
        # for str() and inverse() only
        (self._src_a, self._dst_a,
         self._src_b, self._dst_b) = (src_a, dst_a, src_b, dst_b)
    def __call__(self, point):
        """Transform a point from coordinate system A to B.

        Internally, this is performed in steps:
        - transpose src_a to origo
        - rotate <src_a, src_b> parallell to <dst_a, dst_b>
        - scale so that |src_a-src_b| == |dst_a-dst_b|
        - transpose origo to dst_a
        """
        return self._t2(self._s(self._r(self._t1(point))))
    def inverse(self):
        """Return the inverse of this transform, i.e.
        a transform from the destination to the source.
        """
        return Transform(self._dst_a, self._src_a,
                         self._dst_b, self._src_b)

if __name__ == "__main__":
    import unittest

    class test(unittest.TestCase):
        bridge = (144808, 720870)
        fjelka = (144600, 721200)
        storjen = (145200, 720600)
        maps = ((568,
                 74, 118, 430, 471,
                 197, 312),
                (615,
                 119, 124, 437, 511,
                 221, 329),
                (623,
                 88, 168, 483, 476,
                 233, 346),
                (909,
                 120, 190, 689, 755,
                 316, 500))
        def testSlipsiken(self):
            for h, fx, fy, sx, sy, bx, by in self.maps:
                # 2--3 pixels off may be reasonable with this data
                # print vector.distance(map(self.bridge), (bx, h-by))
                map = Transform(self.fjelka, (fx, h-fy),
                                self.storjen, (sx, h-sy))
                self.assert_(vector.distance(map(self.bridge), (bx, h-by)) < 3)
        def testSlipsiken2(self):
            for h, fx, fy, sx, sy, bx, by in self.maps:
                map = Transform((fx, h-fy), self.fjelka,
                                (sx, h-sy), self.storjen)
                # 10m off may be reasonable with this data
                # print vector.distance(self.bridge, map((bx, h-by)))
                self.assert_(vector.distance(self.bridge, map((bx, h-by))) < 10)
        def testSlipsiken3(self):
            for h, fx, fy, sx, sy, bx, by in self.maps:
                map = Transform(self.fjelka, (fx, h-fy),
                                self.storjen, (sx, h-sy)).inverse()
                # 10m off may be reasonable with this data
                # print vector.distance(self.bridge, map((bx, h-by)))
                self.assert_(vector.distance(self.bridge, map((bx, h-by))) < 10)

    unittest.main()
