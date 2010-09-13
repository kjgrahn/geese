/* -*- c++ -*-
 *
 * $Id: worldfile.h,v 1.2 2010-09-13 22:05:57 grahn Exp $
 *
 * Copyright (c) 2010 Jörgen Grahn <grahn+src@snipabacken.se>
 * All rights reserved.
 */
#ifndef GEESE_WORLDFILE_H
#define GEESE_WORLDFILE_H

#include <iosfwd>
#include <string>

class Transform;

bool parse_world(Transform& t, const std::string& file, std::ostream& log);

#endif
