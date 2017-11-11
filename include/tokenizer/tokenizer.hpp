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

	TokenizeResult tokenize(std::istream& stream);
	TokenizeResult tokenize(const std::string& file);

	namespace {
		std::unique_ptr<Comment> parseComment(LAReader& reader);
		std::unique_ptr<VertexLike> parseVertex(LAReader& reader);
		std::unique_ptr<VertexLike> parseVertexTexture(LAReader& reader);
		std::unique_ptr<VertexLike> parseVertexNormal(LAReader& reader);

		std::unique_ptr<VertexLike> parseVertexLike(LAReader& reader, const TokenType tokenType);

		std::string parseNumber(LAReader& reader);
	}

}

#endif // TOKENIZER_TOKENIZER_HPP