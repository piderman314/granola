#ifndef TOKENIZER_TOKENIZER_HPP
#define TOKENIZER_TOKENIZER_HPP

#include "token.hpp"

#include <istream>
#include <tuple>
#include <vector>

namespace granola::tokenizer {

	using TokenizeResult = std::tuple<std::string, std::vector<Token>>;

	TokenizeResult tokenize(std::istream& stream);
	TokenizeResult tokenize(const std::string& file);

}

#endif // TOKENIZER_TOKENIZER_HPP