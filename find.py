#!/usr/bin/python
# -*- coding: iso-8859-1 -*-
# $Id: find.py,v 1.1 2004-06-27 20:48:09 grahn Exp $
"""Finding a point based on its distance from
several other known points.
"""

import math
from geese import vector, coordinate

def pairs(seq):
    """Return all pairs of elements from the sequence,
    but not both (a,b) and (b,a).
    """
    acc = []
    n = len(seq)
    for i in xrange(0, n-1):
        for j in xrange(i+1, n):
            acc.append((seq[i], seq[j]))
    return acc

def intersection(ra, rb, d):
    """Find one intersection point (x, y), y>=0
    of two circles A and B.

    A is centered at (0, 0) with radius ra.
    B is centered at (d, 0) with radius rb.
    
    We will assume that they *do* intersect,
    i.e. they are not distinct and one is not
    confined inside the other.
    """
    # XXX ugly int overflow workaround
    ra, rb, d = map(float, (ra, rb, d))
    x = d*d + ra*ra - rb*rb
    x /= 2*d
    y = math.sqrt(ra*ra - x*x)
    return x, y

def find2(ca, ra, cb, rb):
    """Find the two points where circles A (centered at xa,ya with radius ra)
    and B (similarly) intersect.

    Returns two (x, y) pairs. 
    """
    d = vector.distance(ca, cb)
    # transpose and rotate down;
    # A in origo and B on the x axis.
    t = coordinate.Transform(ca, vector.origo,
                             cb, (d, 0))
    # .., only we don't have to do the formal transform
    x, y = intersection(ra, rb, d)
    # ... but only revert it
    tinv = t.inverse()
    return (tinv((x, y)), tinv((x, -y)))

def findmany(neighbors):
    """Find a (x, y) coordinate of a point based on a sequence of
    (x, y, d) - neighbor coordinates and their individual distances
    from the desired point.

    Like find(), but returns a sequence of candidates (with the
    obviously 'bad' ones weeded out.
    """
    # OK, so we have the maths in the form of find2() above.  That's
    # not enough since each comparison between two neighbors yields
    # two candidates and one is a fake. Combining all neighbors in N
    # permutatons would yield 2N candidates, out of which N would be
    # nicely clustered around the desired point, with the other N
    # spread in a random fashion (although one could devise an
    # infinite set of neighbors pointing out two desired points).

    # So there's still a need for wild heuristics.

    candidates = []
    for a, b in pairs(neighbors):
        if a==b: continue
        da = a[2]; a = a[:2]
        db = b[2]; b = b[:2]
        candidates.append(find2(a, da,
                                b, db))
    # define the good candidate in each pair as the
    # one minimizing the sum of distance errors, and
    # eliminate the other
    good = []
    for c1, c2 in candidates:
        d1 = 0
        d2 = 0
        for x, y, d in neighbors:
            neighbor = (x, y)
            d1 += abs(d - vector.distance(c1, neighbor))
            d2 += abs(d - vector.distance(c2, neighbor))
        if d1 < d2:
            good.append(c1)
        else:
            good.append(c2)
    return good

def find(neighbors):
    """Find a (x, y) coordinate of a point, based on a sequence of
    (x, y, d) - neighbor coordinates and their individual distances
    from the desired point.

    As a pen and a piece of paper would tell you, two distinct
    neighbor points are probably not enough. Three should be enough in
    all but the most extreme cases, but more is better - especially
    since we do not really trust our inputs.
    """
    # XXX should really do a mean here
    return findmany(neighbors)[0]


if __name__ == "__main__":
    import unittest

    class test(unittest.TestCase):
        def test1(self):
            p = find([(1, 0, 1),
                      (-1, 0, 1),
                      (0, 1, 1)])
            self.assert_(vector.distance(p, (0,0)) < 0.0001)
        def test2(self):
            target = (15000, 15000)
            neighbors = [(16000, 1288.8), (54597.1, 63293),
                         (48826, 44595), (33355, 45136),
                         (30398, 13304), (34431, 49244),
                         (49370, 25516), (  431, 62799),
                         (57039, 63712), ( 6193,  5822),
                         (37736, 55439), (61714, 12498),
                         (28239, 19560), (14100, 17669),
                         (12224, 63443), (39887, 51559),
                         (24136, 44727), (15000, 15000)]
            nds = []
            tx, ty = map(float, target)
            for x, y in neighbors:
                d = math.sqrt((x-tx)*(x-tx) + (y-ty)*(y-ty))
                assert d <= abs(x-tx) + abs(y-ty)
                nds.append((x, y, d))
            # do find() with all of the neighbors, then
            # fewer ... end with only the first three
            while len(nds) > 2:
                p = find(nds)
                #print p
                self.assert_(vector.distance(p, target) < 0.0001)
                del nds[-1]

    unittest.main()
