/* Copyright (c) 2023 Jörgen Grahn.
 * All rights reserved.
 */
#include "parse.h"

#include <cctype>
#include <cstdlib>

template <>
unsigned eat<unsigned>(const char*& p)
{
    char* end;
    unsigned n = std::strtoul(p, &end, 10);
    if (end==p) throw ParseError {};
    p = end;
    return n;
}

template <>
double eat<double>(const char*& p)
{
    char* end;
    double n = std::strtod(p, &end);
    if (end==p) throw ParseError {};
    p = end;
    return n;
}

/**
 * Consume whitespace, and then 's'. Throws ParseError otherwise, so
 * it's like Perl "s/^\s*foo// or die".
 *
 */
void eat(const char*& p, const char* token)
{
    const auto q = p;
    eat_ws(p);
    while (*p && *token) {
	if (*p != *token) break;
	p++; token++;
    }
    if (*token) {
	p = q;
	throw ParseError {};
    }
}

/**
 * Like above, but does nothing on mismatch. You cannot tell
 * afterwards if the optional string was there or not.
 */
void eat_optional(const char*& p, const char* token)
{
    try {
	eat(p, token);
    }
    catch (const ParseError&) {}
}

/**
 * Consume whitespace, i.e. advance 'p' until it doesn't point to
 * whitespace.
 */
void eat_ws(const char*& p)
{
    auto ws = [] (int ch) { return std::isspace(ch); };
    while(*p && ws(*p)) p++;
}
