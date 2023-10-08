#include <parse.h>

#include <orchis.h>

namespace parse {

    using orchis::TC;
    using orchis::assert_eq;

    namespace integer {

	void present(TC)
	{
	    const char* s = "  42foo";
	    unsigned n = eat<unsigned>(s);
	    assert_eq(n, 42);
	    assert_eq(std::string{s}, "foo");
	}

	void absent(TC)
	{
	    const char* s = "  a42foo";
	    try {
		eat<unsigned>(s);
	    }
	    catch (const ParseError&) {
		return;
	    }
	    orchis::fail("should throw ParseError");
	}
    }

    namespace floating {

	void present(TC)
	{
	    const char* s = "  42foo";
	    unsigned n = eat<double>(s);
	    assert_eq(n, 42);
	    assert_eq(std::string{s}, "foo");
	}

	void absent(TC)
	{
	    const char* s = "  a42foo";
	    try {
		eat<double>(s);
	    }
	    catch (const ParseError&) {
		return;
	    }
	    orchis::fail("should throw ParseError");
	}
    }

    namespace mandatory {

	std::string eat(const char* s, const char* prefix)
	{
	    ::eat(s, prefix);
	    return s;
	}

	void assert_mismatch(const char* const s, const char* const token)
	{
	    auto t = s;
	    try {
		::eat(t, token);
	    }
	    catch (const ParseError&) {
		assert_eq(s, t);
		return;
	    }
	    orchis::fail("should throw ParseError");
	}

	void present(TC)
	{
	    assert_eq(eat(" foobar", "foo"), "bar");
	}

	void complete(TC)
	{
	    assert_eq(eat(" foobar", "foobar"), "");
	}

	void absent(TC)
	{
	    assert_mismatch("bar", "foo");
	}

	void absent2(TC)
	{
	    assert_mismatch("fo", "foo");
	    assert_mismatch(" fo", "foo");
	}

	void absent3(TC)
	{
	    assert_mismatch("", "foo");
	}
    }

    namespace optional {

	std::string eat(const char* s, const char* prefix)
	{
	    ::eat_optional(s, prefix);
	    return s;
	}

	void present(TC)
	{
	    assert_eq(eat(" foobar", "foo"), "bar");
	}

	void absent(TC)
	{
	    assert_eq(eat("bar foo", "foo"), "bar foo");
	    assert_eq(eat(" fobar", "foo"), " fobar");
	}
    }

    namespace whitespace {

	void assert_eat(const std::string& head, const std::string& tail)
	{
	    auto s = head + tail;
	    auto p = s.c_str();
	    ::eat_ws(p);
	    orchis::assert_eq(p, tail);
	}

	void empty(TC)
	{
	    assert_eat("", "");
	}

	void nil(TC)
	{
	    assert_eat("", "foo");
	}

	void simple(TC)
	{
	    assert_eat(" ", "foo");
	}

	void varia(TC)
	{
	    assert_eat("  ", "foo");
	    assert_eat("\t", "foo");
	    assert_eat("\r\n", "foo");
	    assert_eat(" ", "foo bar");
	    assert_eat("", "foo bar");
	}
    }
}
