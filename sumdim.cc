/*
 * $Id: sumdim.cc,v 1.3 2011-02-06 07:19:39 grahn Exp $
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
 * The dimensions may turn out as (0, 0) if the file format isn't
 * recognized by anydim. For other errors you have to check the stream
 * state (or the copy of in.bad() that's left as a member).
 */
SumDim::SumDim(std::istream& in)
    : width(0),
      height(0),
      bad(false)
{
    md5::Ctx ctx;
    anydim::AnyDim dim;
    char buf[4096];
    unsigned char* const ubuf = reinterpret_cast<unsigned char*>(buf);
    while(in) {
	in.read(buf, 4096);
	ctx.update(ubuf, in.gcount());
	dim.feed(ubuf, ubuf+in.gcount());
    }
    bad = in.bad();
    if(!bad) {
	sum = ctx.digest();
	width = dim.width;
	height = dim.height;
    }
}
