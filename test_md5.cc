/**
 * $Id: test_md5.cc,v 1.5 2010-09-18 07:53:17 grahn Exp $
 *
 * Copyright (c) 2010 Jörgen Grahn
 * All rights reserved.
 *
 */
#include "md5.h"
#include "md5pp.h"

#include <testicle.h>
#include <string>
#include <vector>
#include <list>
#include <sstream>

namespace {

    void assert_eq(const std::string& ref,
		   const md5::Digest& digest)
    {
	testicle::assert_eq(ref, digest.hex());
    }

    void assert_eq(const std::string& ref,
		   const unsigned char* digest)
    {
	const md5::Digest d(digest);
	assert_eq(ref, d);
    }
}

namespace md5 {

    namespace c {

	void test()
	{
	    MD5_CTX ctx;
	    MD5_Init(&ctx);
	    {
		unsigned char digest[16];
		MD5_CTX c = ctx;
		MD5_Final(digest, &c);
		assert_eq("d41d8cd98f00b204e9800998ecf8427e",
			  digest);
	    }

	    MD5_Update(&ctx, "Hello, world!", 13);
	    {
		unsigned char digest[16];
		MD5_CTX c = ctx;
		MD5_Final(digest, &c);
		assert_eq("6cd3556deb0da54bca060b4c39479839",
			  digest);
	    }

	    MD5_Update(&ctx,
		       "xxxxxxxxxxxxxxxxxxxx"
		       "xxxxxxxxxxxxxxxxxxxx"
		       "xxxxxxxxxxxxx", 53);
	    {
		unsigned char digest[16];
		MD5_CTX c = ctx;
		MD5_Final(digest, &c);
		assert_eq("69bb639b030e67e4c360d0206abca653",
			  digest);
	    }
	}
    }

    namespace cpp {

	const char hw[] = "Hello, world!";

	void test()
	{
	    md5::Ctx ctx;
	    assert_eq("d41d8cd98f00b204e9800998ecf8427e",
		      ctx.digest());

	    ctx.update(hw, 13);
	    assert_eq("6cd3556deb0da54bca060b4c39479839",
		      ctx.digest());

	    ctx.update("xxxxxxxxxxxxxxxxxxxx"
		       "xxxxxxxxxxxxxxxxxxxx"
		       "xxxxxxxxxxxxx", 53);
	    assert_eq("69bb639b030e67e4c360d0206abca653",
		      ctx.digest());
	}

	void test_string()
	{
	    const std::string d(hw);
	    md5::Ctx ctx;
	    ctx.update(d);
	    assert_eq("6cd3556deb0da54bca060b4c39479839",
		      ctx.digest());
	}

	void test_vector()
	{
	    const std::vector<char> d(hw, hw+13);
	    md5::Ctx ctx;
	    ctx.update(d);
	    assert_eq("6cd3556deb0da54bca060b4c39479839",
		      ctx.digest());
	}

	void test_iter()
	{
	    const std::list<unsigned char> d(hw, hw+13);
	    md5::Ctx ctx;
	    ctx.update(d.begin(), d.end());
	    assert_eq("6cd3556deb0da54bca060b4c39479839",
		      ctx.digest());
	}

	void test_istream()
	{
	    std::istringstream is(hw);
	    md5::Ctx ctx;
	    ctx.update(is);
	    assert_eq("6cd3556deb0da54bca060b4c39479839",
		      ctx.digest());
	    testicle::assert_(is.eof());
	}
    }
}
