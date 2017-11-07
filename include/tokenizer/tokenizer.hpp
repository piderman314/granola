#ifndef TOKENIZER_TOKENIZER_HPP
#define TOKENIZER_TOKENIZER_HPP

#include "token.hpp"

#include <istream>
#include <vector>

namespace granola::tokenizer {

	auto tokenize(std::istream& stream);
	auto tokenize(const std::string& file);

}

#endif // TOKENIZER_TOKENIZER_HPP