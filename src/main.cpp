#include <iostream>

#include "config.hpp"
#include "arguments.hpp"
#include "cppres.hpp"

void print_usage(const std::string & program, std::ostream & output) {
	output
		<< "Usage: " << program << "[options] [<resource_1> [<resource_2> ...]]" << std::endl
		<< R"X(
Options:
  -path PATH      base path for resources (cdir)
  -ns NAMESPACE   root namespace for resources (res)
  -version        print program version
  -help           display this help message and exit
  -line-sz SIZE   size of the line (15)
  -header HEADER  resource header file name (resources.hpp)
  -source SOURCE  source file with included resources
                  (resources.cpp)
  -header-only    create only header file
  -source-only    create only source file
  -tree           print filesystem tree for selected
                  resource files to stdout
)X";
}

int main(int argc, char *argv[]) {
	using namespace cppres;
	try {
		arguments.parse(argv + 1, argc - 1);
	} catch (const app_arg_error & e) {
		std::cerr << "argument error: " << e.what() << std::endl;
		print_usage(*argv, std::cerr);
		return 1;
	} catch (const std::exception & e) {
		std::cerr << "unexpected error: " << e.what() << std::endl;
		return 2;
	}
	if (arguments.help) {
		print_usage(*argv, std::cout);
		return 0;
	}
	if (arguments.version) {
		std::cout
			<< "group: " << config::group << std::endl
			<< "project: " << config::project << std::endl
			<< "version: " << config::version << std::endl
			<< "maintainer: " << config::maintainer << std::endl
			<< "build: " << config::build << std::endl;
	}
	cppres_entry();
	return 0;
}
