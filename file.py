#!/usr/bin/env python
# -*- coding: iso-8859-1 -*-
#
# $Id: file.py,v 1.1 2004-08-22 20:17:21 grahn Exp $
#
# Copyright (c) 2004 Jörgen Grahn <jgrahn@algonet.se>
# All rights reserved.
#

import re
import os
import sys
from geese.coordinate import Transform
from geese import segrid

class res:
    "helper REs"
    # 21da8fb51edd4c398765e012b9cc8738
    md5sum = re.compile(r'[\da-z]{32}$')
    # 2146 x 2578
    size = re.compile(r'(\d+)\s*x\s*(\d+)$')
    # 6447181 1356800 -> 0,0
    mapping = re.compile(r'(\d+)\s+(\d+)\s*->\s*(\d+)\s*,\s*(\d+)$')
    comment = re.compile(r'#')
    # 1802,1387 = 183,147,112  #b79370  ( 29  38  71 HSV)  [  325,  135]
    xvpixel = re.compile(r'\s*(\d+),\s*(\d+)\s=')

f = open("/home/grahn/text/koordinater", "r")

for s in f.readlines():
    s = s.strip()
    if s=='' or res.comment.match(s):
        pass
    elif res.md5sum.match(s):
        s
    elif res.mapping.match(s):
        res.mapping.match(s).groups()
    elif res.size.match(s):
        res.size.match(s).groups()
    else:
        print s

togrid = Transform((628, 902), (6447000, 1370000),
                   ( 41, 308), (6449000, 1368000))

os.system('xv /home/grahn/maps/luttra.png &')

for s in sys.stdin.readlines():
    m = res.xvpixel.match(s)
    if m:
        pixel = tuple(map(int, m.groups()))
        #coord = segrid.Point(togrid(pixel))
        print pixel
