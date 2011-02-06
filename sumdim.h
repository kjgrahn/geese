/* -*- c++ -*-
 *
 * $Id: sumdim.h,v 1.2 2011-02-06 07:19:39 grahn Exp $
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

struct SumDim {

    md5::Digest sum;
    unsigned width;
    unsigned height;
    bool bad;
    explicit SumDim(std::istream& in);
};

#endif
