#ifndef TOKENIZER_TOKENIZER_HPP
#define TOKENIZER_TOKENIZER_HPP

#include "token.hpp"
#include "lareader.hpp"

#include <istream>
#include <memory>
#include <tuple>
#include <vector>

namespace granola::tokenizer {

	using TokenizeResult = std::tuple<std::string, Tokens>;

	TokenizeResult tokenize(std::istream& stream) noexcept;
	TokenizeResult tokenize(const std::string& file) noexcept;

	namespace {
		std::unique_ptr<Comment> parseComment(LAReader& reader);
	}

}

#endif // TOKENIZER_TOKENIZER_HPP