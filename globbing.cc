/*
 * $Id: globbing.cc,v 1.1 2010-09-13 22:05:57 grahn Exp $
 *
 * Copyright (c) 2010 Jörgen Grahn <grahn+src@snipabacken.se>
 * All rights reserved.
 */
#include "globbing.h"

#include <glob.h>

std::vector<std::string> glob(const std::string& pattern)
{
    std::vector<std::string> v;
    glob(pattern, v);
    return v;
}


void glob(const std::string& pattern, std::vector<std::string>& v)
{
    static glob_t GL;
    glob_t gl = GL;
    if(!glob(pattern.c_str(), GLOB_TILDE_CHECK, 0, &gl)) {
	for(char** i = gl.gl_pathv; *i; ++i) {
	    v.push_back(*i);
	}
    }
    globfree(&gl);
}
