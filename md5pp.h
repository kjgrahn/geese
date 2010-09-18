/* -*- c++ -*-
 *
 * $Id: md5pp.h,v 1.1 2010-09-18 06:18:13 grahn Exp $
 *
 * Copyright (c) 2003, 2004, 2010 Jörgen Grahn <grahn+src@snipabacken.se>
 * All rights reserved.
 */
#ifndef GEESE_MD5_H
#define GEESE_MD5_H

#include <iosfwd>

namespace md5 {

    struct Digest {
    };
}

std::ostream& operator<< (std::ostream& os, const md5::Digest& val);

#endif
