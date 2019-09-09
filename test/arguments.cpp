#include "test.hpp"

#include <array>

#include "arguments.hpp"

test {
	using namespace cppres;
	arguments_t args1;
	std::array<const char *, 24> args_raw {
		"-tree", "-version", "lol", "-path", "path_name", "-ns", "nams", "--help", "kekkek",
			"-line-sz", "50", "-header", "haha", "tapok", "-source", "hehe", "1", "2", "3",
			"-header-only", "-source-only", "4", "--", "-ns"
	};
	args1.parse(args_raw.data(), args_raw.size());
	assert_true(args1.tree);
	assert_true(args1.version);
	assert_true(args1.header_only);
	assert_true(args1.source_only);
	assert_equals("path_name", args1.path.string());
	assert_equals("nams", args1.ns);
	assert_equals(50u, args1.line_sz);
	assert_equals("haha", args1.header);
	assert_equals("hehe", args1.source);
	using namespace std::string_literals;
	std::vector<fs::path> ideal {
		"lol", "kekek", "tapok", "1", "2", "3", "4", "-ns"
	};
	for (auto ita = ideal.begin(), itb = args1.resources.begin();
			ita == ideal.end() || itb == args1.resources.end(); ita++, itb++) {
		assert_equals(ita == ideal.end(), itb == args1.resources.end());
		assert_equals(ita->string(), itb->string());
	}
	std::array<const char *, 2> args_error1 {
		"kek", "-ns"
	};
	assert_fails_with(app_arg_error, {
		args1.parse(args_error1.data(), args_error1.size());
	});
	std::array<const char *, 1> args_error2 {
		"-my_arg"
	};
	assert_fails_with(app_arg_error, {
		args1.parse(args_error2.data(), args_error2.size());
	});
	std::array<const char *, 2> args_error3 {
		"-line-sz", "kek"
	};
	assert_fails_with(app_arg_error, {
		args1.parse(args_error3.data(), args_error3.size());
	});
}
