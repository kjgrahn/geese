#!/usr/bin/python
# -*- coding: iso-8859-1 -*-
# $Id: find.py,v 1.10 2004-08-07 15:58:10 grahn Exp $
"""Finding a point based on its distance from
several other known points, possibly with minor
errors in the input data.

Note that the approximate nature of this can be a problem -
not because the results are approximate, but because the user
has no way to get a hint about the size of the error.
Use with care.
"""

import math
from geese import vector, coordinate

class Error(Exception):
    """Insufficient data. Too few neighbor points to divine
    the point we're looking for, or too few of them are distinct
    and don't contradict each other.
    """

class _IntersectError(Exception):
    """No point fullfils the criteria (i.e. because the reference
    points are too far apart).
    """

def _pairs(seq):
    """Return all pairs of elements from the sequence,
    but not both (a,b) and (b,a).
    """
    acc = []
    n = len(seq)
    for i in xrange(0, n-1):
        for j in xrange(i+1, n):
            acc.append((seq[i], seq[j]))
    return acc

def _mean(seq):
    """Return the mean of a sequence of points (using possibly not the
    best definition of two-dimensional mean.
    """
    n = float(len(seq))
    xs = 0
    ys = 0
    for x, y in seq:
        xs += x
        ys += y
    return xs/n, ys/n

class _Cluster:
    """A set of points, hopefully clustered together.
    """
    def __init__(self):
        self.points = []
        self.accx = 0
        self.accy = 0
    def add(self, p):
        self.points.append(p)
        x, y = p
        self.accx += x
        self.accy += y
    def __len__(self):
        return len(self.points)
    def center(self):
        """The center of the cluster (kind of).
        An empty cluster is by convention centered at origo.
        """
        n = len(self.points)
        if n==0:
            return vector.origo
        n = float(n)
        return self.accx/n, self.accy/n
    def spread(self):
        """A value indicating how tight or spread the cluster is,
        currently the square root of the area of the smallest
        rectangle, parallel to the axis, enclosing all points.
        """
        minx, miny = self.points[0]
        maxx, maxy = self.points[0]
        for x, y in self.points:
            if x<minx: minx = x
            if x>maxx: maxx = x
            if y<miny: miny = y
            if y>maxy: maxy = y
        return math.sqrt((maxx-minx)*(maxy-miny))
    def distance2(self, p):
        """The square of the distance between [the center] of the
        cluster and p.
        """
        return vector.distance2(self.center(), p)

def _uniq(seq):
    """Remove duplicates - and fsck up the ordering - of a sequence.
    Only works when the elements are sequences, and they are turned
    into tuples as they are returned.
    """
    acc = {}
    for e in seq:
        acc[tuple(e)] = 1
    return acc.keys()

def _intersection(ra, rb, d):
    """Find one intersection point (x, y), y>=0
    of two circles A and B.

    A is centered at (0, 0) with radius ra.
    B is centered at (d, 0) with radius rb.
    
    We will assume that they *do* intersect,
    i.e. they are not distinct and one is not
    confined inside the other. At least the former case
    raises _IntersectError.
    """
    # XXX ugly int overflow workaround
    ra, rb, d = map(float, (ra, rb, d))
    x = d*d + ra*ra - rb*rb
    x /= 2*d
    try:
        y = math.sqrt(ra*ra - x*x)
    except ValueError:
        raise _IntersectError
    return x, y

def find2(ca, ra, cb, rb):
    """Find the two points where circles A (centered at xa,ya with radius ra)
    and B (similarly) intersect.

    Returns two (x, y) pairs, or raises _IntersectError.
    """
    d = vector.distance(ca, cb)
    # We could raise error here by noticing that
    # d > ra+rb, but rounding errors below could
    # still fsck us ... so don't do that.

    # transpose and rotate down;
    # A in origo and B on the x axis.
    t = coordinate.Transform(ca, vector.origo,
                             cb, (d, 0))
    # .., only we don't have to do the formal transform
    x, y = _intersection(ra, rb, d)
    # ... but only revert it
    tinv = t.inverse()
    return (tinv((x, y)), tinv((x, -y)))

def findmany(neighbors):
    """Find a (x, y) coordinate of a point based on a sequence of
    (x, y, d) - neighbor coordinates and their individual distances
    from the desired point.

    Like find(), but returns a sequence of candidates (with the
    obviously 'bad' ones weeded out.

    May raise Error.
    """
    # OK, so we have the maths in the form of find2() above.  That's
    # not enough since each comparison between two neighbors yields
    # two candidates and one is a fake.

    # Combining all neighbors in N permutations would yield 2N
    # candidates, out of which N would be nicely clustered around the
    # desired point, with the other N hopefully spread in a seemingly
    # random fashion.  The worst case is when the neighbors are on a
    # line; we then get two tight clusters of N candidates each. The
    # way this algorithm tends to be used, that's not an unlikely
    # scenario. Murphy's law applies.

    # So the strategy is to maintain two sets of candidates, and for
    # each pair of candidates, add one to each set in a way which
    # maximises the 'clustering' aspect. If, at the end, we have more
    # than one candidate in each set, and one of them is significantly
    # better clustered than the other, that is the one centered around
    # the point we're looking for.

    cluster1 = _Cluster()
    cluster2 = _Cluster()
    for a, b in _pairs(_uniq(neighbors)):
        assert a!=b
        da = a[2]; a = a[:2]
        db = b[2]; b = b[:2]
        try:
            c1, c2 = find2(a, da, b, db)
            d11 = cluster1.distance2(c1)
            d12 = cluster1.distance2(c2)
            d21 = cluster2.distance2(c1)
            d22 = cluster2.distance2(c2)
            mind = min(d11, d12, d21, d22)
            if mind==d11 or mind==d22:
                cluster1.add(c1)
                cluster2.add(c2)
            else:
                cluster1.add(c2)
                cluster2.add(c1)                
        except _IntersectError:
            pass
    assert len(cluster1)==len(cluster2)
    if len(cluster1) < 2:
        raise Error, 'not enough good neighbors'

    # heuristic: the cluster with the most spread loses.  If that
    # cluster is not spread more than a third of the distance between
    # the two clusters, /we/ lose because we cannot safely say which
    # cluster is the correct one.

    if cluster2.spread() < cluster1.spread():
        cluster1, cluster2 = cluster2, cluster1
    dc = vector.distance(cluster1.center(), cluster2.center())
    if cluster2.spread() * 3 < dc:
        raise Error, 'two sets of likely candidates'
    return cluster1.points

def find(neighbors):
    """Find a (x, y) coordinate of a point, based on a sequence of
    (x, y, d) - neighbor coordinates and their individual distances
    from the desired point.

    As a pen and a piece of paper would tell you, two distinct
    neighbor points are not enough. Three or more are needed - but if
    they are all on a straight line, we fail to find the point no
    matter how many neighbors there are.

    Raises Error when the point cannot be identified.
    """
    return _mean(findmany(neighbors))


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
        def testBroken(self):
            neighbors = [(6450000, 1360000, 3109),
                         (6452000, 1360000, 3116),
                         (6450000, 1362000, 1369),
                         (6452000, 1362000, 1385),
                         (6454000, 1362000, 3156),
                         (6450000, 1364000, 1443),
                         (6452000, 1364000, 1459),
                         (6454000, 1364000, 3189),
                         (6452000, 1366000, 3215)]
            find(neighbors)
        def testNonIntersect(self):
            self.assertRaises(_IntersectError,
                              find2,
                              (6450000, 1360000), 3109,
                              (6452000, 1366000), 3215)
            self.assertRaises(Error,
                              find,
                              [(6450000, 1360000, 3109),
                               (6452000, 1366000, 3215)])
            self.assertRaises(Error, find, [])

        def assertClose(self, p1, p2):
            "helper, raises unless p1 and p2 are within 20 (meters)."
            msg = '%s and %s are close' % (map(int, p1),
                                           map(int, p2))
            self.assert_(vector.distance(p1, p2) < 20, msg)

        def testJala(self):
            self.assertRaises(Error, find,
                              [(6436000, 1360000, 4206), (6436000, 1360000, 4206),
                               (6438000, 1360000, 4242), (6438000, 1360000, 4242),
                               (6440000, 1360000, 5129)])
            p2 = find([(6436000, 1350000, 5967), (6436000, 1350000, 5967),
                       (6436000, 1352000, 4003), (6436000, 1352000, 4003),
                       (6436000, 1354000, 2108), (6436000, 1354000, 2108),
                       (6436000, 1356000,  928), (6436000, 1356000,  928),
                       (6436000, 1358000, 2297), (6436000, 1358000, 2297),
                       (6436000, 1360000, 4206), (6436000, 1360000, 4206),
                       (6438000, 1350000, 5993), (6438000, 1350000, 5993),
                       (6438000, 1352000, 4042), (6438000, 1352000, 4042),
                       (6438000, 1354000, 2180), (6438000, 1354000, 2180),
                       (6438000, 1356000, 1082), (6438000, 1356000, 1082),
                       (6438000, 1358000, 2363), (6438000, 1358000, 2363),
                       (6438000, 1360000, 4242), (6438000, 1360000, 4242),
                       (6440000, 1350000, 6650), (6440000, 1352000, 4964),
                       (6440000, 1352000, 4964), (6440000, 1354000, 3614),
                       (6440000, 1354000, 3614), (6440000, 1356000, 3078),
                       (6440000, 1356000, 3078), (6440000, 1358000, 3727),
                       (6440000, 1358000, 3727), (6440000, 1360000, 5129)])
            p3 = find([(6440000, 1350000, 6650), (6440000, 1352000, 4964),
                       (6440000, 1352000, 4964), (6440000, 1354000, 3614),
                       (6440000, 1354000, 3614), (6440000, 1356000, 3078),
                       (6440000, 1356000, 3078), (6440000, 1358000, 3727),
                       (6440000, 1358000, 3727), (6440000, 1360000, 5129),
                       (6442000, 1354000, 5419), (6442000, 1354000, 5419),
                       (6442000, 1356000, 5078), (6442000, 1356000, 5078),
                       (6442000, 1358000, 5495), (6442000, 1358000, 5495)])
            self.assertClose(p2, p3)
        def testMonarp(self):
            self.assertRaises(Error, find,
                              [(6440000, 1362000, 4415), (6440000, 1364000, 4405),
                               (6440000, 1364000, 4405), (6440000, 1366000, 5227)])
            p2 = find([(6440000, 1362000, 4415), (6440000, 1364000, 4405),
                       (6440000, 1364000, 4405), (6440000, 1366000, 5227),
                       (6442000, 1358000, 5521), (6442000, 1360000, 3795),
                       (6442000, 1360000, 3795), (6442000, 1362000, 2513),
                       (6442000, 1362000, 2513), (6442000, 1364000, 2495),
                       (6442000, 1364000, 2495), (6442000, 1366000, 3760),
                       (6442000, 1366000, 3760), (6444000, 1358000, 5030),
                       (6444000, 1358000, 5030), (6444000, 1360000, 3036),
                       (6444000, 1360000, 3036), (6444000, 1362000, 1064),
                       (6444000, 1362000, 1064), (6444000, 1364000, 1021),
                       (6444000, 1364000, 1021), (6444000, 1366000, 2992),
                       (6444000, 1366000, 2992), (6446000, 1358000, 5303),
                       (6446000, 1358000, 5303), (6446000, 1360000, 3469),
                       (6446000, 1360000, 3469), (6446000, 1362000, 1986),
                       (6446000, 1362000, 1986), (6446000, 1364000, 1964),
                       (6446000, 1364000, 1964), (6446000, 1366000, 3431),
                       (6446000, 1366000, 3431)])
        def testMonarpMonarp(self):
            self.assertRaises(Error, find,
                              [(6440000, 1362000, 3962), (6440000, 1364000, 3546),
                               (6440000, 1366000, 4178)])
            p2 = find([(6440000, 1362000, 3962), (6440000, 1364000, 3546),
                       (6440000, 1366000, 4178), (6442000, 1360000, 4081),
                       (6442000, 1362000, 2353), (6442000, 1364000, 1555),
                       (6442000, 1366000, 2701), (6442000, 1368000, 4492),
                       (6444000, 1360000, 3807), (6444000, 1362000, 1838),
                       (6444000, 1364000,  509), (6444000, 1366000, 2267),
                       (6444000, 1368000, 4244), (6446000, 1360000, 4509),
                       (6446000, 1362000, 3036), (6446000, 1364000, 2469),
                       (6446000, 1366000, 3313)])
        def testSmula(self):
            p1 = find([(6436000, 1364000, 4553), (6436000, 1364000, 4553),
                       (6436000, 1366000, 2563), (6436000, 1366000, 2563),
                       (6436000, 1368000,  634), (6436000, 1368000,  634),
                       (6436000, 1370000, 1494), (6436000, 1370000, 1494),
                       (6438000, 1364000, 4839), (6438000, 1364000, 4839),
                       (6438000, 1366000, 3041), (6438000, 1366000, 3041),
                       (6438000, 1368000, 1755), (6438000, 1368000, 1755),
                       (6438000, 1370000, 2216), (6438000, 1370000, 2216),
                       (6440000, 1366000, 4464), (6440000, 1366000, 4464),
                       (6440000, 1368000, 3709), (6440000, 1368000, 3709),
                       (6440000, 1370000, 3949), (6440000, 1370000, 3949)])
            self.assertRaises(Error, find,
                              [(6440000, 1366000, 4464), (6440000, 1366000, 4464),
                               (6440000, 1368000, 3709), (6440000, 1368000, 3709),
                               (6440000, 1370000, 3949), (6440000, 1370000, 3949),
                               (6440000, 1372000, 5042)])
        def testTornarp(self):
            self.assertRaises(Error, find,
                              [(6436000, 1360000, 4092), (6436000, 1360000, 4092),
                               (6436000, 1362000, 4176), (6436000, 1362000, 4176),
                               (6436000, 1364000, 5112)])
            self.assertRaises(Error, find,
                              [(6436000, 1356000, 6273), (6436000, 1356000, 6273),
                               (6436000, 1358000, 4904), (6436000, 1358000, 4904),
                               (6436000, 1360000, 4092), (6436000, 1360000, 4092)])
        def testVarnum(self):
            self.assertRaises(Error, find,
                              [(6440000, 1346000, 6528), (6440000, 1348000, 6214),
                               (6440000, 1350000, 6527)])
            p2 = find([(6440000, 1350000, 6527), (6442000, 1350000, 4663),
                       (6442000, 1352000, 5808), (6444000, 1350000, 2981),
                       (6444000, 1352000, 4569), (6446000, 1350000, 2008),
                       (6446000, 1352000, 4002)])
        def testAsarp(self):
            p1 = find([(6436000, 1362000, 5677), (6436000, 1364000, 3685),
                       (6436000, 1364000, 3685), (6436000, 1366000, 1711),
                       (6436000, 1366000, 1711), (6436000, 1368000,  525),
                       (6436000, 1368000,  525), (6436000, 1370000, 2371),
                       (6436000, 1370000, 2371), (6438000, 1364000, 4380),
                       (6438000, 1364000, 4380), (6438000, 1366000, 2922),
                       (6438000, 1366000, 2922), (6438000, 1368000, 2426),
                       (6438000, 1368000, 2426), (6438000, 1370000, 3352),
                       (6438000, 1370000, 3352), (6440000, 1366000, 4706),
                       (6440000, 1368000, 4415), (6440000, 1368000, 4415),
                       (6440000, 1370000, 4984), (6440000, 1370000, 4984)])
            self.assertRaises(Error, find,
                              [(6440000, 1366000, 4706), (6440000, 1368000, 4415),
                               (6440000, 1368000, 4415), (6440000, 1370000, 4984),
                               (6440000, 1370000, 4984)])

    unittest.main()
