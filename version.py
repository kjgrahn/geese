#!/usr/bin/env python
# -*- coding: iso-8859-1 -*-
#
# $Id: version.py,v 1.1 2005-01-04 22:48:51 grahn Exp $
#
"""Maintains two strings: 'name' and 'version'; the package's
name and version. These are taken from CVS's dollar-Name keyword.
"""

def cvs_says(dollarname='$Name:  $'):
    """Parse a CVS tag name according to the convention
    'product_name-major-minor-subminor-...'
    into a tuple: the name and the dot-delimited version number.
    
    Both become 'unknown' if the CVS tag isn't what we'd like it to be.
    """
    import re
    m = re.match(r'\$'r'Name:\s+(.+?)-(\d+(-\d+)*)\D', dollarname)
    if not m: return ('unknown', 'unknown')
    return m.group(1), m.group(2).replace('-', '.')

name, version = cvs_says()
