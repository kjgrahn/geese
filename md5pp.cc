/*
 * $Id: md5pp.cc,v 1.2 2010-09-18 07:29:22 grahn Exp $
 *
 * Copyright (c) 2010 Jörgen Grahn <grahn+src@snipabacken.se>
 * All rights reserved.
 */
#include "md5pp.h"

#include <iostream>

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


std::ostream& operator<< (std::ostream& os, const Digest& val)
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


Digest Ctx::digest() const
{
    Digest d;
    /* MD5_Final() modifies (resets?) the context */
    MD5_CTX tmp = val;
    MD5_Final(d.val, &tmp);
    return d;
}
