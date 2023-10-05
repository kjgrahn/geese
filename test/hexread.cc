#include <hexread.h>

#include <orchis.h>

namespace hex {

    using orchis::TC;

    std::string str(const std::vector<uint8_t>& v)
    {
	std::string s;
	for (unsigned n : v) {
	    char buf[3];
	    std::snprintf(buf, sizeof buf, "%02x", n);
	    s += buf;
	}
	return s;
    }

    void assert_reads(std::string s,
		      const std::vector<uint8_t>& v,
		      const std::string& tail = "")
    {
	s += tail;
	const char* p = s.c_str();
	std::vector<uint8_t> vv(s.size()/2);
	size_t n = hexread(vv.data(), &p, p+s.size());
	vv.resize(n);

	orchis::assert_eq(str(vv), str(v));
	orchis::assert_eq(p, tail);
    }

    void empty(TC)
    {
	assert_reads("", {}, "");
	assert_reads("", {}, "foo");
	assert_reads("  ", {}, "foo");
    }

    void three(TC)
    {
	const std::vector<uint8_t> v {0x12, 0x34, 0x56};
	assert_reads("12 34 56", v);
	assert_reads("1234 56", v);
	assert_reads("123456", v);
    }

    void whitespace(TC)
    {
	const auto v = hexread("12 34 56");
	assert_reads(" 12 34 56", v);
	assert_reads("  12 34 56", v);
	assert_reads("\t12 34 56", v);
    }

    void trailer(TC)
    {
	const auto v = hexread("12 34 56");
	assert_reads("12 34 56", v, "foo");
	assert_reads("123456   ", v, "x");
    }

    void trailer2(TC)
    {
	const auto v = hexread("12");
	assert_reads("12", v, "3 456");
    }

    void uppercase(TC)
    {
	const auto v = hexread("0abc def1");
	assert_reads("0abc def1", v);
	assert_reads("0ABC DEF1", v);
	assert_reads("0AbC dEf1", v);
    }
}
