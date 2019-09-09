#include "cppres.hpp"

#include <fstream>
#include <iostream>

#include "arguments.hpp"
#include "scanner.hpp"

namespace cppres {

void cppres_entry() {
	res_tree_node_t root = scanner(arguments.path, arguments.resources);

	if (arguments.tree) {
		std::cout << std::string(root) << std::endl;
	}

	std::ostream * header;
	if (!arguments.source_only)
		header = new std::ofstream(arguments.header);
	else
		header = nullptr;

	std::ostream * source;
	if (!arguments.header_only)
		source = new std::ofstream(arguments.source);
	else
		source = nullptr;
	
	if (header)
		place_header_prefix(*header);
	if (source)
		place_source_prefix(*source);
	
	fill_object object { header, source, "\t" };
	root.fill(object, arguments.path, 1);

	if (header)
		place_header_suffix(*header);
	if (source)
		place_source_suffix(*source);
	
	delete header;
	delete source;
}

} // namespace cppres
