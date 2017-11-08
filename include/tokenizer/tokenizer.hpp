#ifndef TOKENIZER_TOKENIZER_HPP
#define TOKENIZER_TOKENIZER_HPP

#include "token.hpp"
#include "lareader.hpp"

#include <istream>
#include <memory>
#include <tuple>
#include <vector>

namespace granola::tokenizer {

	using TokenizeResult = std::tuple<std::string, std::vector<std::unique_ptr<Token>>>;

	TokenizeResult tokenize(std::istream& stream);
	TokenizeResult tokenize(const std::string& file);

	namespace {
		std::unique_ptr<Comment> parseComment(LAReader& reader);
	}

}

#endif // TOKENIZER_TOKENIZER_HPP