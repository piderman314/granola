#include "tokenizer/token.hpp"

namespace granola::tokenizer {

	Token::Token(const TokenType tokenType) : tokenType{ tokenType } {}

	Comment::Comment(const std::string&& comment) : Token{ TokenType::Comment }, comment{ std::move(comment) } {}

	Vertex::Vertex(const std::string&& x, const std::string&& y, const std::string&& z) : Token{ TokenType::Vertex }, x{ x }, y{ y }, z{ z } {}
}