/* -*- c++ -*-
 *
 * $Id: split.h,v 1.2 2011-06-12 20:51:44 grahn Exp $
 *
 * Copyright (c) 2011 Jörgen Grahn
 * All rights reserved.
 */
#ifndef GEESE_SPLIT_H
#define GEESE_SPLIT_H

#include <string>
#include <vector>

std::vector<std::string> split(const char* a, const char* b);
std::vector<std::string> split(const std::string& s);

#endif
