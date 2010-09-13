/*
 * $Id: worldfile.cc,v 1.2 2010-09-13 22:05:57 grahn Exp $
 *
 * Copyright (c) 2010 Jörgen Grahn <grahn+src@snipabacken.se>
 * All rights reserved.
 */
#include "worldfile.h"
#include "transform.h"

#include <iostream>
#include <string>

bool parse_world(Transform& t, const std::string& file, std::ostream& log)
{
    log << file << ": XXX no such luck\n";
    return false;
}
