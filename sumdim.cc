/*
 * $Id: sumdim.cc,v 1.1 2011-02-06 06:54:54 grahn Exp $
 *
 * Copyright (c) 2011 Jörgen Grahn <grahn+src@snipabacken.se>
 * All rights reserved.
 */
#include "sumdim.h"

#include <anydim.h>
#include <iostream>

/**
 * Given an istream of an image file, calculate its
 * MD5 digest and find the image dimensions, in one pass.
 *
 * The dimensions may turn out as (0, 0) if the file
 * format isn't recognized by anydim. For other errors
 * you have to check the stream state.
 */
void sum_and_dim(std::istream& in,
		 md5::Digest& sum,
		 unsigned& width,
		 unsigned& height)
{
}
