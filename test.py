#!/usr/bin/env python
# -*- coding: iso-8859-1 -*-
#
# $Id: test.py,v 1.1 2010-09-18 20:08:12 grahn Exp $
#
# Copyright (c) 2010 Jörgen Grahn <grahn+src@snipabacken.se>
# All rights reserved.
#
import coordinate
import find
import library
import segrid
import transform
import vector

if __name__ == "__main__":
    import unittest

    # I don't care any longer what the right way is
    # to do this ...
    test_coordinate = coordinate.test
    test_find = find.test
    test_library = library.testMap
    test_segrid = segrid.test
    test_transform = transform.test
    test_vector = vector.test

    unittest.main()
