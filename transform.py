#!/usr/bin/python

from vector import vector


class scale:
    def __init__(self, s = 1):
        self._s = s
    def __call__(self, v):
        s = self._s
        return vector(s*v.x, s*v.y)

class transpose:
    def __init__(self, v = vector(0,0)):
        self._v = v
    def __call__(self, v):
        dx = self._v.x
        dy = self._v.y
        return vector(v.x+dx, v.y+dy)

class rotate:
    def __init__(self, a = 0):
        self._sin = sin(a)
        self._cos = cos(a)
    def __call__(self, v):
        sin = self._sin
        cos = self._cos
        x = v.x
        y = v.y
        return vector(cos*x - sin*y,
                      sin*x + cos*y)

