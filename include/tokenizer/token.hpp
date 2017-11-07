#ifndef TOKENIZER_TOKEN_HPP
#define TOKENIZER_TOKEN_HPP

#include <string>

namespace granola::tokenizer {

	enum class TokenType {
		Comment,
		Vertex,
		VertexTexture,
		VertexNormal
	};

	class Token {
		const TokenType tokenType;

	public:
		Token(const TokenType tokenType);
	};

	class Comment : Token {
		const std::string& comment;

	public:
		Comment(const std::string& comment);
	};

}

#endif // TOKENIZER_TOKEN_HPP