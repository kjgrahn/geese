/*
 * $Id: md5pp.cc,v 1.5 2011-02-06 15:05:54 grahn Exp $
 *
 * Copyright (c) 2010, 2011 Jörgen Grahn <grahn+src@snipabacken.se>
 * All rights reserved.
 */
#include "md5pp.h"

#include "hexread.h"

#include <iostream>
#include <cstdio>

using namespace md5;


Digest::Digest(const unsigned char buf[16])
{
    std::copy(buf, buf+16, val);
}


std::string Digest::hex() const
{
    std::string s;
    s.reserve(32);
    for(const unsigned char* p = val; p != val+16; ++p) {
	char buf[3];
	std::sprintf(buf, "%02x", *p);
	s.append(buf);
    }
    return s;
}


bool Digest::operator== (const Digest& other) const
{
    return std::equal(std::begin(val), std::end(val),
		      std::begin(other.val));
}


bool Digest::operator!= (const Digest& other) const
{
    return !(*this==other);
}


/**
 * Parse a string, which must contain only a hex MD5 digest and
 * whitespace.
 */
Digest md5::parse(const std::string& s)
{
    const char* p = s.c_str();
    std::vector<uint8_t> v(s.size()/2);
    size_t n = hexread(v.data(), &p, p+s.size());

    Digest d;
    if (n==16) {
	std::copy(begin(v), end(v), std::begin(d.val));
    }

    return d;
}


std::ostream& md5::operator<< (std::ostream& os, const Digest& val)
{
    return os << val.hex();
}


Ctx::Ctx()
{
    MD5_Init(&val);
}


Ctx& Ctx::update(const char* data, unsigned long size)
{
    MD5_Update(&val, data, size);
    return *this;
}

Ctx& Ctx::update(const unsigned char* data, unsigned long size)
{
    MD5_Update(&val, data, size);
    return *this;
}

Ctx& Ctx::update(const std::string& data)
{
    return update(data.data(), data.size());
}

Ctx& Ctx::update(const std::vector<char>& data)
{
    if(!data.empty()) {
	/* std::vector<T> in practice works like this,
	 * and that is also being standardized.
	 */
	update(&data[0], data.size());
    }
    return *this;
}

Ctx& Ctx::update(const std::vector<unsigned char>& data)
{
    if(!data.empty()) {
	update(&data[0], data.size());
    }
    return *this;
}


/**
 * Checksum a stream (e.g. an open file).
 */
Ctx& Ctx::update(std::istream& in)
{
    char buf[4096];
    while(in) {
	in.read(buf, 4096);
	update(buf, in.gcount());
    }
    return *this;
}


Digest Ctx::digest() const
{
    Digest d;
    /* MD5_Final() modifies (resets?) the context */
    MD5_CTX tmp = val;
    MD5_Final(d.val, &tmp);
    return d;
}
