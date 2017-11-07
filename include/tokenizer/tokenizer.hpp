#include "token.hpp"

#include <istream>
#include <vector>

namespace granola::tokenizer {

	auto tokenize(const std::istream& stream);
	auto tokenize(const std::string& file);

}