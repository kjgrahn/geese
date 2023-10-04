#include <library.h>
#include <files...h>

#include <orchis.h>

using orchis::assert_eq;
using orchis::assert_true;
using orchis::assert_false;

namespace {

    Library<Map> parse(const char* s)
    {
	std::stringstream ss {s};
	Files files {ss};
	std::ostringstream err;
	const auto lib = parse_lib(files, err);
	std::string tail;
	orchis::assert_false(files.getline(tail));
	orchis::assert_eq(err.str(), "");
	return lib;
    }

    void assert_has(const Library<Map>& lib, const char* name)
    {
	orchis::assert_true(lib.count(name));
    }

    void assert_map(const Library<Map>& lib, const char* const name,
		    const std::vector<md5::Digest>& checksums,
		    unsigned width, unsigned height)
    {
	assert_true(lib.count(name));
	const Map& map = lib.find(name)->second;
	assert_false(map.empty);
	assert_eq(map.checksums.size(), 1);
	assert_true(map.checksums == checksums);
	assert_eq(map.dimensions.width, width);
	assert_eq(map.dimensions.height, height);
    }
}

namespace library {

    using orchis::TC;

    void simple(TC)
    {
	const Library<Map> lib = parse("luttra_n.png\n"
				       "b3f1b2e3fcafeb2c14f43d7261e0d62a\n"
				       "847 x 367\n"
				       "6450000 1368000 -> 183 336\n"
				       "6451000 1370000 -> 775  43\n");
	assert_eq(lib.size(), 2);
	assert_has(lib, "luttra_n.png");
	assert_has(lib, "b3f1b2e3fcafeb2c14f43d7261e0d62a");

	assert_map(lib, "luttra_n.png",
		   {md5::parse("b3f1b2e3fcafeb2c14f43d7261e0d62a")},
		   847, 367);
    }

    void variations(TC)
    {
	const Library<Map> lib = parse("# no checksum\n"
				       "foo\n"
				       "847 x 367\n"
				       "6450000 1368000 -> 183 336\n"
				       "6451000 1370000 -> 775  43\n"
				       "\n"
				       "# two names\n"
				       "bar\n"
				       "baz\n"
				       "b3f1b2e3fcafeb2c14f43d7261e0d62a\n"
				       "847 x 367\n"
				       "6450000 1368000 -> 183 336\n"
				       "6451000 1370000 -> 775  43\n"
				       "\n"
				       "# commas in transformations\n"
				       "bat\n"
				       "847 x 367\n"
				       "6450000 1368000 -> 183, 336\n"
				       "6451000 1370000 -> 775,  43\n");

	assert_eq(lib.size(), 1+3+1);
	assert_has(lib, "foo");
	assert_has(lib, "bar");
	assert_has(lib, "baz");
	assert_has(lib, "b3f1b2e3fcafeb2c14f43d7261e0d62a");
	assert_has(lib, "bat");
    }
}
