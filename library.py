#!/usr/bin/python
# -*- coding: iso-8859-1 -*-
# $Id: library.py,v 1.1 2004-08-19 21:42:54 grahn Exp $

from geese import coordinate

class Map:
    """Info on a specific image mapping a specific area.
    """
    def __init__(names, checksums, dimension,
                 world_a, map_a, world_b, map_b) :
        self.names = names
        self.checksums = checksums
        self.dimension = dimension
        self.into = coordinate.Transform(world_a, map_a, world_b, map_b)
        self.outof = self.into.inverse()
