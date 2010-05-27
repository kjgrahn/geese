#!/usr/bin/env python
# -*- coding: iso-8859-1 -*-
#
# $Id: segrid.py,v 1.5 2010-05-27 21:16:13 grahn Exp $
#
# Copyright (c) 2004 Jörgen Grahn <grahn+src@snipabacken.se>
# All rights reserved.
#
"""Coordinates in the Rikets Nät, aka the Swedish Grid.

For our purposes, this is a square grid on a flat earth, preserving
angles and distances. This is probably almost true for sufficiently
small areas.

A coordinate in Rikets Nät is usually denoted in print as 64457 13620
(five-digit form, last digit is 100m, error ±50m) or 6445760 1362000
(seven-digit form, last digit is 1m, error ±0.5m). These different
formats can be mixed without risking confusion, since the valid ranges
are all in [10000 .. 99000].

The two first digits are (or were) frequently set
in a smaller point size.

Continuing with the example 64457 13620, 64457 is named the 'x
coordinate' and 13620 the 'y coordinate'.  The 'x' coordinate grows
towards the north, the 'y' coordinate towards the east. Yes, this is
terribly confusing for someone used to the normal cartesian
terminology.  We will call them 'north' and 'east' values, and give
them in that order.

 |     north   east
 |    (64457, 13620)
 |..+
 |  .
 |  .
 |  .
 |  .
 -----------

At one time there seems to have been a convention to truncate rather
than round coordinate values, so that a coordinate indicated the
lower-left corner of a square (x, y, x+1, y+1). This module instead
assumes rounding.

XXX Or possibly truncation is still the norm?
"""

import math

class Point:
    """A point in the Rikets Nät.
    """
    def __init__(self, north, east):
        """Create a point based on its north and east values.

        Any input resolution can be used and it is unified here to
        one-metre resolution. Thus, the following are equivalent:

        Point(64457,     13620)
        Point(6445700,   1362000)
        Point(6445700.0, 1362000.0)

        However, the original resolution is remembered for printing
        purposes.
        """
        i = 1
        while north < 1000000:
            i *= 10
            north *= 10
            east *= 10
        self.north = north
        self.east = east
        self.resolution = i
    def __str__(self):
        ne = (self.north / self.resolution,
              self.east / self.resolution)
        return 'Point(%d, %d)' % ne
    def __cmp__(self, other):
        return cmp((self.north, self.east),
                   (other.north, other.east))
    def tstr(self, kind=0):
        """Return the point as a troff(1) source string, with
        point size changing commands as appropriate.
        """
        acc = []
        for n in (self.north, self.east):
            n /= self.resolution
            n = str(n)
            if kind==0:
                acc.append(r'\s-2%s\s0%s' % (n[:2], n[2:]))
            else:
                acc.append(r'\s-2%s\s0' % n)
        return ' '.join(acc)


if __name__ == "__main__":
    import unittest

    class test(unittest.TestCase):
        def testConstruct(self):
            for n, e in [(64457,     13620),
                         (6445700,   1362000),
                         (6445700.0, 1362000.0)]:
                p = Point(n, e)
                self.assertEqual(p, Point(64457, 13620))
        def testStr(self):
            self.assertEqual(str(Point(6445, 1362)),
                             'Point(6445, 1362)')
            self.assertEqual(str(Point(644500, 136200)),
                             'Point(644500, 136200)')
        def testTstr(self):
            self.assertEqual(Point(64457, 13620).tstr(),
                             r'\s-264\s0457 \s-213\s0620')
            self.assertEqual(Point(6445700, 1362000).tstr(),
                             r'\s-264\s045700 \s-213\s062000')

    unittest.main()
