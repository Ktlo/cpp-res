#include "arguments.hpp"

#include <cstring>
#include <iterator>

namespace cppres {

arguments_t arguments;

using namespace std::literals;

void check_argn(std::size_t n, const char * curr) {
	if (n <= 1)
		throw app_arg_error("argument -"s + curr + " has no value");
}

void arguments_t::parse(const char * const* str, std::size_t n) {
	bool special = true;
	for (; n; n--, str++) {
		const char * curr = *str;
		if (special && *curr == '-') {
			curr++;
			if (!std::strcmp(curr, "path")) {
				check_argn(n, curr);
				path = str[1];
				n--;
				str++;
			} else if (!std::strcmp(curr, "ns")) {
				check_argn(n, curr);
				ns = str[1];
                n--;
                str++;
			} else if (!std::strcmp(curr, "header")) {
				check_argn(n, curr);
				header = str[1];
                n--;
                str++;
			} else if (!std::strcmp(curr, "source")) {
				check_argn(n, curr);
				source = str[1];
                n--;
                str++;
			} else if (!std::strcmp(curr, "line-sz")) {
				check_argn(n, curr);
				try {
					line_sz = std::stoi(str[1]);
				} catch (...) {
					throw app_arg_error("can't parse \""s + str[1] + "\" as number");
				}
                n--;
                str++;
			} else if (!std::strcmp(curr, "-"))
				special = false;
			else if (!std::strcmp(curr, "version"))
				version = true;
			else if (!std::strcmp(curr, "help") || !std::strcmp(curr, "-help"))
				help = true;
			else if (!std::strcmp(curr, "tree"))
				tree = true;
			else if (!std::strcmp(curr, "header-only"))
				header_only = true;
			else if (!std::strcmp(curr, "source-only"))
				source_only = true;
			else
				throw app_arg_error("no such argument: -"s + curr);
		} else {
			resources.emplace_back(curr);
		}
	}
}

} // namespace cppres
