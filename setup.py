#!/usr/bin/env python
# -*- coding: iso-8859-1 -*-
#
# $Id: setup.py,v 1.3 2004-07-12 21:58:51 grahn Exp $
#
from distutils.core import setup

setup(name = 'geese',
      version = '1.0',
      description = 'manipulate coordinates and coordinate systems on bitmapped maps',

      author = 'Jörgen Grahn',
      author_email = 'jgrahn@algonet.se',
      license = 'BSD',
      url = 'http://www.algonet.se/~jgrahn/comp/',

      packages = ['geese'],

      package_dir = {'geese': ''})
