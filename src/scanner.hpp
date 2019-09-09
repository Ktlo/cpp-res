#ifndef _SCANNER_HEAD
#define _SCANNER_HEAD

#include <filesystem>
#include <ostream>
#include <vector>
#include <unordered_map>
#include <memory>

namespace fs = std::filesystem;

namespace cppres {

struct fill_object {
	std::ostream * header;
	std::ostream * source;
	const std::string & prefix;
};

struct res_tree_node_t {
	std::unordered_map<std::string, std::unique_ptr<res_tree_node_t>> branches;
	void place(fs::path::iterator & iter, const fs::path::iterator & end);
	void fill(fill_object & object, const fs::path & path, unsigned tab);
	operator std::string() const;
private:
	void stringify(std::string & str, unsigned tab) const;
};

res_tree_node_t scanner(const fs::path & dir, const std::vector<fs::path> & resources);

void place_header_prefix(std::ostream & output);
void place_source_prefix(std::ostream & output);

void place_header_suffix(std::ostream & output);
void place_source_suffix(std::ostream & output);

} // namespace cppres

#endif // _SCANNER_HEAD
