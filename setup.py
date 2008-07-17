#!/usr/bin/env python
# -*- coding: iso-8859-1 -*-
#
# $Id: setup.py,v 1.10 2008-07-17 11:09:21 grahn Exp $
#
from distutils.core import setup

def cvs_says(dollarname='$Name:  $'):
    import re
    m = re.match(r'\$'r'Name:\s+(.+?)-(\d+(-\d+)*)\D', dollarname)
    if not m: return ('geese', 'unknown')
    return m.group(1), m.group(2).replace('-', '.')

name, version = cvs_says()

setup(name = name,
      version = version,
      description = 'manipulate coordinates and coordinate systems on bitmapped maps',

      author = 'Jörgen Grahn',
      author_email = 'grahn+src@snipabacken.se',
      license = 'GPL',
      url = 'http://snipabacken.se/~grahn/comp/',

      packages = ['geese'],
      
      package_dir = {'geese': ''})
