/* -*- c++ -*-
 *
 * $Id: worldfile.h,v 1.1 2010-09-12 21:06:41 grahn Exp $
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
bool find_world(Transform& t, const std::string& mapfile, std::ostream& log);

#endif
