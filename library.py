#!/usr/bin/python
# -*- coding: iso-8859-1 -*-
# $Id: library.py,v 1.5 2004-09-12 16:52:34 grahn Exp $

import re
from geese import coordinate
from geese import segrid
from geese import vector

class Map:
    """Info on a specific image mapping a specific area.
    """
    def __init__(self,
                 names, checksums, dimension,
                 world_a, map_a, world_b, map_b) :
        self.names = names
        self.checksums = checksums
        self.dimension = dimension
        w, h = dimension
        wax, way = map_a
        wbx, wby = map_b
        self.into = coordinate.Transform(world_a, (wax, way),
                                         world_b, (wbx, wby))
        self.outof = self.into.inverse()
    def coordOf(self, x, y):
        """Find the world coordinate of pixel (x, y).
        These are rounded to integers.
        """
        return tuple(map(lambda i: int(i+.5),
                         self.outof((x,y))))
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
