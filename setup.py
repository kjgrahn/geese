#!/usr/bin/env python
# -*- coding: iso-8859-1 -*-
#
# $Id: setup.py,v 1.6 2004-09-07 21:10:45 grahn Exp $
#
from distutils.core import setup

def cvs_says(dollarname='$Name:  $'):
    import re
    m = re.match(r'\$'r'Name:\s+(.+?)-(\d+(-\d+)*)\D', dollarname)
    return m.group(1), m.group(2).replace('-', '.')

name, version = cvs_says()

setup(name = name,
      version = version,
      description = 'manipulate coordinates and coordinate systems on bitmapped maps',

      author = 'Jörgen Grahn',
      author_email = 'jgrahn@algonet.se',
      license = 'GPL',
      url = 'http://www.algonet.se/~jgrahn/comp/',

      packages = ['geese'],
      scripts = ['geese_pick'],
      
      package_dir = {'geese': ''})
