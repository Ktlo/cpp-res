#ifndef _ARGUMENTS_HEAD
#define _ARGUMENTS_HEAD

#include <string>
#include <filesystem>
#include <vector>
#include <cinttypes>
#include <stdexcept>

namespace fs = std::filesystem;

namespace cppres {

struct app_arg_error : public std::runtime_error {
	explicit app_arg_error(const std::string & message) : std::runtime_error(message) {}
};

struct arguments_t {
	fs::path path = fs::current_path();
	std::string ns = "res";
	std::string header = "resources.hpp";
	std::string source = "resources.cpp";
	bool version = false;
	bool help = false;
	bool tree = false;
	bool header_only = false;
	bool source_only = false;
	unsigned line_sz = 15;
	std::vector<fs::path> resources;
	void parse(const char * const* str, std::size_t n);
};

extern arguments_t arguments;

} // namespace cppres

#endif // _ARGUMENTS_HEAD
