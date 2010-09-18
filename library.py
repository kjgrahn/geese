#!/usr/bin/python
# -*- coding: iso-8859-1 -*-
# $Id: library.py,v 1.11 2010-09-18 20:08:12 grahn Exp $

import re
from geese import coordinate
from geese import segrid
from geese import vector

class Map:
    """Info on a specific image mapping a specific area.
    - one or more names (probably file names)
    - zero or more md5sums
    - dimensions (in pixels)
    - a mapping to and from a real-word coordinate system
    and data derived from those things.

    World coordinates are on the form (north, east) - see
    geese.segrid.

    Pixel coordinates are by ancient convention (x, y) with
    y growing downwards from an origo in the upper-left
    corner of the pixmap.
    """
    def __init__(self,
                 names, checksums, dimension,
                 world_a, map_a, world_b, map_b) :
        self.names = names
        self.checksums = checksums
        self.dimension = dimension
        w, h = dimension
        self.into = coordinate.Transform(world_a, map_a,
                                         world_b, map_b)
        self.outof = self.into.inverse()
    def coordOf(self, x, y):
        """Find the world coordinate of pixel (x, y).
        These are rounded to integers.
        """
        return tuple(map(lambda i: int(i+.5),
                         self.outof((x,y))))
    def pixelOf(self, north, east):
        """Find the pixel of this world coordinate,
        as a (x, y) tuple of floats.
        """
        return self.into((north, east))
    def contains(self, p):
        """True if the map contains coordinate p.
        """
        w, h = self.dimension
        x, y = self.into(p)
        if x<0 or x>=w: return 0
        if y<0 or y>=h: return 0
        return 1
    def scale(self):
        """The scale of the map as a floating-point number,
        as in 'one pixel is n meters wide'.
        """
        return vector.distance(self.outof((0,0)),
                               self.outof((0,1)))
    def area(self):
        """The area in square meters covered by the map.
        """
        w, h = self.dimension
        w = vector.distance(self.outof((0,0)),
                               self.outof((w,0)))
        h = vector.distance(self.outof((0,0)),
                            self.outof((0,h)))
        return w*h
    def __str__(self):
        names = self.names
        if len(names) > 1:
            return '%s (%s)' % (names[0],
                                ', '.join(names[1:]))
        return names[0]

def parse(f):
    class _res:
        "helper REs"
        # 21da8fb51edd4c398765e012b9cc8738
        md5sum = re.compile(r'[\da-z]{32}$')
        # 2146 x 2578
        size = re.compile(r'(\d+)\s*x\s*(\d+)$')
        # 6447181 1356800 -> 0,0
        mapping = re.compile(r'(\d+)\s+(\d+)\s*->\s*(\d+)\s*,\s*(\d+)$')
        comment = re.compile(r'#')
    maps = []
    md5sums = []
    size = None
    mappings = []
    names = []
    for s in f.readlines():
        s = s.strip()
        if s=='':
            if size and mappings:
                ((wa, ma), (wb, mb)) = mappings
                maps.append(Map(names, md5sums, size,
                                wa, ma, wb, mb))
            md5sums = []
            size = None
            mappings = []
            names = []
        elif _res.comment.match(s):
            pass
        elif _res.md5sum.match(s):
            md5sums.append(s)
        elif _res.mapping.match(s):
            a, b, c, d = map(float, _res.mapping.match(s).groups())
            mappings.append(((a,b),(c,d)))
        elif _res.size.match(s) and not size:
            size = map(int, _res.size.match(s).groups())
        else:
            names.append(s)
    if size:
        ((wa, ma), (wb, mb)) = mappings
        maps.append(Map(names, md5sums, size,
                        wa, ma, wb, mb))
    return maps

if 1:
    import unittest
    
    class testMap(unittest.TestCase):
        plains = (Map(['plain'], [],
                      (100,100),
                      (6446000, 1360000), (0, 100),
                      (6447000, 1360000), (0,   0)),
                  Map(['plain2'], [],
                      (100,100),
                      (6447000, 1360000), (0, 0),
                      (6446000, 1361000), (100, 100)),
                  Map(['plain3'], [],
                      (100,100),
                      (6446000, 1360000), (0, 100),
                      (6446500, 1360000), (0,  50)))
        rots = (Map(['rotated'], [],
                    (100,100),
                    (6446000, 1360000), (0,   0),
                    (6447000, 1360000), (100, 0)),
                Map(['rotated2'], [],
                    (100,100),
                    (6447000, 1360000), (100, 0),
                    (6446000, 1361000), (0, 100)),
                Map(['rotated3'], [],
                    (100,100),
                    (6446000, 1360000), ( 0, 0),
                    (6446500, 1360000), (50, 0)))
        def assertMaps(self, map, world, pixel):
            "ok iff 'map' maps back & forth between 'world' and 'pixel'"
            n, e = world
            x, y = pixel
            self.assert_(vector.distance(map.pixelOf(n, e),
                                         pixel) < 1,
                         '%s != %s' % (map.pixelOf(n, e), pixel))
            self.assert_(vector.distance(map.coordOf(x, y),
                                         world) < 1,
                         '%s ~= %s' % (map.coordOf(x, y), world))
        def test1(self):
            for map in self.plains:
                self.assertMaps(map, (6447000, 1360000), (  0,   0))
                self.assertMaps(map, (6446000, 1360000), (  0, 100))
                self.assertMaps(map, (6446000, 1361000), (100, 100))
                self.assertMaps(map, (6447000, 1361000), (100,   0))
                self.assertEqual(map.scale(), 10.0)
                self.assertEqual(map.area(), 1.0e6)
        def test2(self):
            for map in self.rots:
                self.assertMaps(map, (6447000, 1360000), (100,   0))
                self.assertMaps(map, (6446000, 1360000), (  0,   0))
                self.assertMaps(map, (6446000, 1361000), (  0, 100))
                self.assertMaps(map, (6447000, 1361000), (100, 100))
                self.assertEqual(map.scale(), 10.0)
                self.assertEqual(map.area(), 1.0e6)

if __name__ == "__main__":
    unittest.main()
