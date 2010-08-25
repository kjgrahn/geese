#!/usr/bin/env python
# -*- coding: iso-8859-1 -*-
#
# $Id: world.py,v 1.2 2010-08-25 19:19:27 grahn Exp $
#
# Copyright (c) 2010 Jörgen Grahn <grahn+src@snipabacken.se>
# All rights reserved.
#
"""Distorting coordinate systems (scaling, rotation, transformation)
using a six-parameter affine transformation (the special case with
'square pixels'.
This is the same thing as a GIS [[World file]], again the square-pixel
special case.
"""

from geese import transform, vector

class World(object):
    """An affine transformation
          
    x' = [ A  B ] x = [ C ]
         [ D  E ]     [ F ]

    with only scaling, rotation and transposition.
    """
    def __init__(self, a,b,c,d,e,f):
        self.a = a
        self.b = b
        self.c = c
        self.d = d
        self.e = e
        self.f = f

    def write(self, f):
        """Write in the so-called [[World file]] format
        to open file 'f'.
        """
        for n in (self.a, self.b, self.c, self.d,
                  self.e, self.f):
            f.write('%.4f\n' % n)

def WorldOf(src_a, dst_a,
            src_b, dst_b):
    """The World defined by a->a', b->b'.
    """
    r = vector.angle(dst_a, dst_b) - vector.angle(src_a, src_b)

if __name__ == "__main__":
    import unittest

    class test(unittest.TestCase):
        pass

    unittest.main()
