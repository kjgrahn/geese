/* -*- c++ -*-
 *
 * $Id: regex.h,v 1.2 2010-09-11 07:51:50 grahn Exp $
 *
 * regex.h
 *
 * Copyright (C) 2001 Jörgen Grahn <jgrahn@algonet.se>
 * Copyright (C) 2010 Jörgen Grahn
 * All rights reserved.
 *----------------------------------------------------------------------------
 *
 *----------------------------------------------------------------------------
 */
#ifndef REGEX_H
#define REGEX_H

#include <sys/types.h>
#include <regex.h>

#include <string>
#include <cassert>

/**
 * A simple wrapper around the basic parts of
 * POSIX Extended Regular Expressions.
 *
 */
class Regex
{
public:
    explicit Regex(const std::string& regex) {
	int rc = regcomp(&_preg, regex.c_str(), REG_EXTENDED|REG_NOSUB|REG_ICASE);
	assert(!rc);
    }
    ~Regex() {regfree(&_preg);}

    bool match(const std::string& s) const {
	return !regexec(&_preg, s.c_str(), 0, 0, 0);
    }

private:
    Regex();
    Regex(const Regex&);

    regex_t _preg;
};

#endif
