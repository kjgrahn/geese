/* -*- c++ -*-
 *
 * $Id: xvpixel.h,v 1.1 2010-09-12 16:00:12 grahn Exp $
 *
 * Copyright (c) 2010 Jörgen Grahn <grahn+src@snipabacken.se>
 * All rights reserved.
 */
#ifndef GEESE_XVPIXEL_H
#define GEESE_XVPIXEL_H

#include <string>

class Pixel;

Pixel xvpixel(const std::string& s, bool& err);

#endif
