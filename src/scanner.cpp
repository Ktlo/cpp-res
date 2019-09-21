#include "scanner.hpp"

#include <stdexcept>
#include <cassert>
#include <fstream>
#include <iostream>
#include <algorithm>

#include "arguments.hpp"

namespace cppres {

void res_tree_node_t::place(fs::path::iterator & iter, const fs::path::iterator & end) {
	if (iter == end)
		return;
	std::string path = iter->string();
	const auto & next = branches.find(path);
	if (next == branches.end()) {
		auto next_node = branches.emplace(std::move(path), std::make_unique<res_tree_node_t>());
		assert(next_node.second);
		next_node.first->second->place(++iter, end);
	} else {
		next->second->place(++iter, end);
	}
}

void tabulate(std::ostream & output, const std::string & prefix, unsigned count) {
	for (unsigned i = 0; i < count; i++) {
		output << prefix;
	}
}

inline std::string underscorify(std::string str) {
	for (char & c : str) {
		if (!std::isalnum(c))
			c = '_';
	}
	return str;
}

inline char int2hex(char c) {
	if (c < 10)
		return '0' + c;
	else
		return 'A' + c - 10;
}

void fill_file(fill_object & object, const fs::path & next, const std::string & name, unsigned tab) {
	auto file_sz = fs::file_size(next);
	if (object.header) {
		auto & header = *object.header;
		tabulate(header, object.prefix, tab);
		header
			<< "extern const std::array<std::uint8_t, " << file_sz << "> " << name << ';' << std::endl
			<< std::endl;
	}
	if (object.source) {
		auto & source  = *object.source;
		tabulate(source, object.prefix, tab);
		source << "const std::array<std::uint8_t, " << file_sz << "> " << name << " {";
		std::ifstream file(next);
		if (!file_sz) {
			std::cerr << "file " << next << " was empty" << std::endl;
				source << " };" << std::endl << std::endl;
			return;
		}
		source << std::endl;
		std::size_t i = 0;
		tabulate(source, object.prefix, tab);
		source << "  ";
		std::size_t received = 0;
		while (true) {
			int c = file.get();
			assert(c >= 0);
			source << "0x" << int2hex((c >> 4) & 15) << int2hex(c & 15);
			i++;
			received++;
			if (received != file_sz)
				source << ',';
			else {
				source << std::endl;
				tabulate(source, object.prefix, tab);
				source << "}; // file " << next.filename() << std::endl << std::endl;
				std::cerr << "file " << next << " included as resource" << std::endl;
				return;
			}
			if (i == arguments.line_sz) {
				i = 0;
				source << std::endl;
				tabulate(source, object.prefix, tab);
				source << "  ";
			} else {
				source << ' ';
			}
		}
	}
}

void res_tree_node_t::fill(fill_object & object, const fs::path & path, unsigned tab) {
	for (auto & pair : branches) {
		fs::path next = path/pair.first;
		std::string name = underscorify(pair.first);
		if (fs::is_directory(next)) {
			if (object.header) {
				auto & header = *object.header;
				tabulate(header, object.prefix, tab);
				header << "namespace " << name << " {" << std::endl << std::endl;
			}
			if (object.source) {
				auto & source = *object.source;
				tabulate(source, object.prefix, tab);
				source << "namespace " << name << " {" << std::endl << std::endl;
			}
			pair.second->fill(object, next, tab + 1);
			if (object.header) {
				auto & header = *object.header;
				tabulate(header, object.prefix, tab);
				header << "} // subdir " << name << std::endl << std::endl;
			}
			if (object.source) {
				auto & source = *object.source;
				tabulate(source, object.prefix, tab);
				source << "} // subdir " << name << std::endl << std::endl;
			}
		} else if (fs::is_regular_file(next)) {
			fill_file(object, next, name, tab);
		} else {
			std::cerr << "file " << next << " is not a regular file" << std::endl;
		}
	}
}

res_tree_node_t::operator std::string() const {
	std::string result = "{\n";
	for (const auto & node : branches) {
		result += "\t" + node.first;
		node.second->stringify(result, 2);
		result += "\n";
	}
	return result + "}";
}

void res_tree_node_t::stringify(std::string & str, unsigned tab) const {
	if (branches.empty())
		return;
	str += ": {\n";
	for (const auto & node : branches) {
		for (unsigned i = 0; i < tab; i++) {
			str += "\t";
		}
		str += node.first;
		node.second->stringify(str, tab + 1);
		str += "\n";
	}
	for (unsigned i = 1; i < tab; i++) {
		str += "\t";
	}
	str += "}";
}

res_tree_node_t scanner(const fs::path & dir, const std::vector<fs::path> & resources) {
	fs::path dir_canon = fs::weakly_canonical(dir);
	res_tree_node_t root;
	for (const fs::path & res : resources) {
		fs::path res_canon = fs::weakly_canonical(res);
		fs::path::iterator ita, itb;
		for (ita = dir_canon.begin(), itb = res_canon.begin();
			ita != dir_canon.end() && itb != res_canon.end() && *ita == *itb;
			++ita, ++itb);
		root.place(itb, res_canon.end());
	}
	return root;
}

std::string upper_underscorify(const std::string & str) {
	std::string under_str = underscorify(str);
	std::transform(under_str.begin(), under_str.end(), under_str.begin(),
		[](unsigned char c){ return std::toupper(c); });
	return under_str;
}

void place_common_prefix(std::ostream & output) {
	output
		<< "namespace " << underscorify(arguments.ns) << " {" << std::endl
		<< std::endl;
}

void place_header_prefix(std::ostream & output) {
	std::string header = upper_underscorify(arguments.header) + "_RES";
	output
		<< "#ifndef _" << header << std::endl
		<< "#define _" << header << std::endl
		<< std::endl
		<< "#include <array>" << std::endl
		<< "#include <cinttypes>" << std::endl
		<< std::endl;
	place_common_prefix(output);
}

void place_source_prefix(std::ostream & output) {
	output
		<< "#include \"" << arguments.header << '"' << std::endl
		<< std::endl;
	place_common_prefix(output);
}

void place_common_suffix(std::ostream & output) {
	output
		<< "} // namespace " << underscorify(arguments.ns) << std::endl;
}

void place_header_suffix(std::ostream & output) {
	place_common_suffix(output);
	output
		<< std::endl
		<< "#endif // " << upper_underscorify(arguments.header) << "_RES" << std::endl;
}

void place_source_suffix(std::ostream & output) {
	place_common_suffix(output);
}

} // namespace cppres
