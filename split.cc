/*
 * $Id: split.cc,v 1.2 2011-06-12 19:55:34 grahn Exp $
 *
 * Copyright (c) 2011 Jörgen Grahn <grahn+src@snipabacken.se>
 * All rights reserved.
 */
#include "split.h"

static
std::vector<std::string> split(const char* a, const char* b)
{
    std::vector<std::string> v;
    return v;
}

std::vector<std::string> split(const std::string& s)
{
    const char* a = &s[0];
    return split(a, a+s.size());
}
