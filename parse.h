/* Copyright (c) 2023 Jörgen Grahn.
 * All rights reserved.
 */
#ifndef GEESE_PARSE_H
#define GEESE_PARSE_H

struct ParseError {};

/**
 * Like strtoul(3) and friends, for selected types T.
 * Consumes whitespace followed by a string representation of T,
 * and updates p. Throws on failure.
 */
template <class T>
T eat(const char*& p);

void eat(const char*& p, const char*);
void eat_optional(const char*& p, const char*);
void eat_ws(const char*& p);

#endif
