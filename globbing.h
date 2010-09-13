/* -*- c++ -*-
 *
 * $Id: globbing.h,v 1.1 2010-09-13 22:05:57 grahn Exp $
 *
 * Copyright (c) 2010 Jörgen Grahn <grahn+src@snipabacken.se>
 * All rights reserved.
 */
#ifndef GEESE_GLOBBING_H
#define GEESE_GLOBBING_H

#include <string>
#include <vector>

std::vector<std::string> glob(const std::string& pattern);
void glob(const std::string& pattern, std::vector<std::string>& v);

#endif
