#!/usr/bin/env python
# -*- coding: iso-8859-1 -*-
#
# $Id: file.py,v 1.2 2004-09-02 20:54:08 grahn Exp $
#
# Copyright (c) 2004 Jörgen Grahn <jgrahn@algonet.se>
# All rights reserved.
#

import re
import os
import os.path
import getopt
import sys
import re
from geese import library

prog = os.path.split(sys.argv[0])[1]
usage = 'usage: %s -f coordinate-file map-file' % prog
try:
    opts, files = getopt.getopt(sys.argv[1:], 'f:')
    if len(files) != 1:
        raise ValueError, 'needs a single file name'
    file, = files
    coords = None
    for option, value in opts:
        if option == '-f': coords = value
    if not coords:
        raise ValueError, 'argument -f missing'
except (ValueError, getopt.GetoptError), s:
    print >>sys.stderr, 'error:', s
    print >>sys.stderr, usage
    sys.exit(1)

coords = library.parse(open(coords, "r"))
for m in coords:
    print m
    if os.path.basename(file) in m.names:
        themap = m
        break

# 1802,1387 = 183,147,112  #b79370  ( 29  38  71 HSV)  [  325,  135]
xvpixel = re.compile(r'\s*(\d+),\s*(\d+)\s=')

os.system('xv %s &' % file)

while 1:
    s = sys.stdin.readline()
    if not s: break
    m = xvpixel.match(s)
    if not m: continue
    x, y = map(int, m.groups())
    print themap.coordOf(x, y)
