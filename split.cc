/*
 * $Id: split.cc,v 1.3 2011-06-12 20:51:44 grahn Exp $
 *
 * Copyright (c) 2011 Jörgen Grahn <grahn+src@snipabacken.se>
 * All rights reserved.
 */
#include "split.h"

#include <algorithm>
#include <cctype>

/**
 * Split [a, b) on whitespace, after taking "quoting" into account.
 * Leading and trailing whitespace gets trimmed.
 *
 */
std::vector<std::string> split(const char* a, const char* b)
{
    std::vector<std::string> v;

    while(a!=b && std::isspace(*a)) ++a;

    unsigned quotes = std::count(a, b, '"');

    while(a!=b && !isspace(*a)) {

	if(*a=='"' && quotes>1) {
	    ++a;
	    const char* p = std::find(a, b, '"');
	    v.push_back(std::string(a, p));
	    quotes-=2;
	    a = p+1;
	    while(a!=b && std::isspace(*a)) ++a;
	    continue;
	}

	const char* const p = a;
	while(a!=b && !std::isspace(*a)) ++a;
	v.push_back(std::string(p, a));
	while(a!=b && std::isspace(*a)) ++a;
    }

    return v;
}

std::vector<std::string> split(const std::string& s)
{
    const char* a = &s[0];
    return split(a, a+s.size());
}
