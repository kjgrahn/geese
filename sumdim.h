/* -*- c++ -*-
 *
 * $Id: sumdim.h,v 1.1 2011-02-06 06:54:54 grahn Exp $
 *
 * Copyright (c) 2011 Jörgen Grahn <grahn+src@snipabacken.se>
 * All rights reserved.
 */
#ifndef GEESE_SUMDIM_H
#define GEESE_SUMDIM_H

#include <iosfwd>
#include <string>
#include <vector>

#include "md5pp.h"

void sum_and_dim(std::istream& in,
		 md5::Digest& sum,
		 unsigned& width,
		 unsigned& height);

#endif
