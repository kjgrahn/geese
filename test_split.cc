/**
 * $Id: test_split.cc,v 1.2 2011-06-12 19:52:04 grahn Exp $
 *
 * Copyright (c) 2011 J�rgen Grahn
 * All rights reserved.
 *
 */
#include "split.h"

#include <testicle.h>

namespace {

    void assert_splits_to(const std::string& s,
			  const std::vector<std::string>& ref)
    {
	using std::vector;
	using std::string;

	const vector<string> v = split(s);
	vector<string>::const_iterator vi = v.begin();
	vector<string>::const_iterator ri = ref.begin();

	while(vi!=v.end() && ri!=ref.end()) {
	    testicle::assert_eq(*vi, *ri);
	    ++vi; ++ri;
	}

	testicle::assert_eq(v.size(), ref.size());
    }

    void assert_splits_to(const std::string& s,
			  const std::string& a,
			  const std::string& b)
    {
	using std::vector;
	using std::string;

	const vector<string> v = split(s);
	testicle::assert_eq(v.size(), 2);
	testicle::assert_eq(v[0], a);
	testicle::assert_eq(v[1], b);
    }
}

namespace split_test {

    using std::string;

    namespace simple {
	static const char* raw[] = {"foo", "bar", "baz", "bat", "fred"};
	static const std::vector<string> ref(raw, raw+sizeof(raw)/sizeof(raw[0]));

	void test1() { assert_splits_to("foo bar baz bat", ref); }
	void test2() { assert_splits_to("foo bar baz bat\n", ref); }
	void test3() { assert_splits_to("foo  bar \tbaz bat", ref); }
	void test4() { assert_splits_to("  foo bar baz bat  \t\n", ref); }
    }

    namespace quoting {
	void test1() { assert_splits_to("foo bar",            "foo", "bar"); }
	void test2() { assert_splits_to("foo \"bar\"",        "foo", "bar"); }
	void test3() { assert_splits_to("\"foo\" \"bar\"",    "foo", "bar"); }
	void test4() { assert_splits_to(" \"foo\"  \"bar\" ", "foo", "bar"); }
	void test5() { assert_splits_to("foo \"bar", 	      "foo", "\"bar"); }
	void test6() { assert_splits_to("\"foo bar", 	      "\"foo", "bar"); }
    }

    namespace spacing {
	void test1() { assert_splits_to("\"f o\" bar",    "f o", "bar"); }
	void test2() { assert_splits_to("\"f o o\" bar",  "f o o", "bar"); }
	void test3() { assert_splits_to("foo \"b r\"",    "foo", "b r"); }
	void test4() { assert_splits_to("foo   \"b r\" ", "foo", "b r"); }
    }
}
