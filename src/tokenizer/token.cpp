#include "tokenizer/token.hpp"

#include <iostream>

namespace granola::tokenizer {

	Token::Token(const TokenType tokenType) : tokenType{ tokenType } {}

	Comment::Comment(const std::string& comment) : Token{ TokenType::Comment }, comment{ comment } {}

	Comment::Comment(const std::string&& comment) : Token{ TokenType::Comment }, comment{ std::move(comment) } {}


}