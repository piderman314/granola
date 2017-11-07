#pragma once

#include "token.hpp"

#include <istream>
#include <vector>

namespace granola::tokenizer {

	auto tokenize(std::istream& stream);
	auto tokenize(const std::string& file);

}