#include "test.hpp"

#include <filesystem>
#include <fstream>

#include "arguments.hpp"
#include "scanner.hpp"
#include "cppres.hpp"

namespace fs = std::filesystem;

test {
	fs::create_directories("res/entry");
	std::ofstream("res/file.txt")
		<< "awadfcervtghbtyjnuhmilmikujyhtgrcfexd" << std::endl
		<< "fscdgvfhbgjhknjgfhdgcfs ncvfggbdcdxsg" << std::endl
		<< "4563456578654678908765__-3rerteetsb35";
	std::ofstream("res/entry/empty.txt");
	std::ofstream("res/entry/a.txt") << "1";

	using namespace cppres;

	auto tree = scanner("res", { "res/file.txt", "res/entry/empty.txt", "res/entry/a.txt" });
	std::cout << std::string(tree) << std::endl;

	assert_equals(2u, tree.branches.size());
	assert_false(tree.branches.end() == tree.branches.find("file.txt"));
	assert_false(tree.branches.end() == tree.branches.find("entry"));
	auto & entry = tree.branches["entry"];
	assert_false(entry->branches.end() == entry->branches.find("empty.txt"));
	assert_false(entry->branches.end() == entry->branches.find("a.txt"));

	arguments.resources = { "res/file.txt", "res/entry/empty.txt", "res/entry/a.txt" };
	arguments.path = "res";
	cppres_entry();
}
