#include "tokenizer/token.hpp"

namespace granola::tokenizer {

	Token::Token(const TokenType tokenType) : tokenType{ tokenType } {}

	Comment::Comment(const std::string& comment) : Token(TokenType::Comment), comment{ comment } {}

}