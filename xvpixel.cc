/*
 * $Id: xvpixel.cc,v 1.1 2010-09-12 16:00:12 grahn Exp $
 *
 * Copyright (c) 2010 Jörgen Grahn <grahn+src@snipabacken.se>
 * All rights reserved.
 */
#include "xvpixel.h"
#include "transform.h"

#include <cstdlib>


/* Parse a string assumed to be cut-and-pasted from xv's
 * pixel-picking function; extract the primary pixel coordinate.
 * Sets 'err' to true if the parsing fails.
 */
Pixel xvpixel(const std::string& s, bool& err)
{
    Pixel pixel(0, 0);
    err = true;

    do {
	/*  773, 846 = 252,254,2... */
	const char* p = s.c_str();
	char* end;
	unsigned x = std::strtoul(p, &end, 10);
	if(end==p || *end!=',') break;
	p = end + 1;
	unsigned y = std::strtoul(p, &end, 10);
	if(end==p || *end!=' ') break;
	p = end;
	while(std::isspace(*p)) ++p;
	if(*p != '=') break;

	pixel = Pixel(x, y);
	err = false;

    } while(0);

    return pixel;
}
