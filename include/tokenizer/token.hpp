#ifndef TOKENIZER_TOKEN_HPP
#define TOKENIZER_TOKEN_HPP

#include <memory>
#include <string>
#include <vector>

namespace granola::tokenizer {

	enum class TokenType {
		Comment,
		Vertex,
		VertexTexture,
		VertexNormal
	};

	class Token {
	public:
		const TokenType tokenType;

	public:
		explicit Token(const TokenType tokenType);
	};

	class Comment : public Token {
	public:
		const std::string comment;

	public:
		explicit Comment(const std::string&& comment);
	};

	class Vertex : public Token {
	public:
		const std::string x;
		const std::string y;
		const std::string z;

	public:
		explicit Vertex(const std::string&& x, const std::string&& y, const std::string&& z);
	};

	using Tokens = std::vector<std::unique_ptr<Token>>;

}

#endif // TOKENIZER_TOKEN_HPP