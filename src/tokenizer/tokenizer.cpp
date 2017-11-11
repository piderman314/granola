#include "tokenizer/tokenizer.hpp"

#include "tokenizer/lareader.hpp"

#include <fstream>
#include <istream>
#include <doctest.h>

#ifndef DOCTEST_CONFIG_DISABLE
#include <sstream>
#endif

namespace granola::tokenizer {

	namespace {
		const char PLUS_CHAR = '+';
		const char MINUS_CHAR = '-';
		const char PERIOD_CHAR = '.';

		const CharPredicate DIGIT = [](char c) { return c >= '0' && c <= '9'; };
		const CharPredicate NUMBER_START = [](char c) { return DIGIT(c) || c == PLUS_CHAR || c == MINUS_CHAR; };
		const CharPredicate NUMBER_CONTINUED = [](char c) { return DIGIT(c) || c == PERIOD_CHAR; };
	}

	TokenizeResult tokenize(std::istream& stream) {
		LAReader reader{ stream };

		Tokens tokens;

		try {
			while (!reader.atEos()) {
				reader.skipWhitespace();

				if (reader.mayBe('#')) {
					tokens.push_back(parseComment(reader));
					continue;
				}

				if (reader.mayBe('v')) {
					tokens.push_back(parseVertex(reader));
					continue;
				}

				reader.skipLine();
			}
		} catch (const LAReaderException& re) {
			return std::make_tuple(re.message, std::move(tokens));
		}

		return std::make_tuple(std::string(""), std::move(tokens));
	}

	TokenizeResult tokenize(const std::string& file) {
		if (file == "") {
			return std::make_tuple("", Tokens());
		}

		std::ifstream file_input{ file };
		return tokenize(file_input);
	}

	namespace {
		std::unique_ptr<Comment> parseComment(LAReader& reader) {
			reader.mustRead('#');
			reader.skipWhitespace();

			return std::make_unique<Comment>(reader.readLine());
		}

		std::unique_ptr<Vertex> parseVertex(LAReader& reader) {
			reader.mustRead('v');
			reader.skipWhitespace();

			reader.skipWhitespace();
			std::string x = parseNumber(reader);
			reader.skipWhitespace();
			std::string y = parseNumber(reader);
			reader.skipWhitespace();
			std::string z = parseNumber(reader);
			reader.skipLine();

			return std::make_unique<Vertex>(std::move(x), std::move(y), std::move(z));
		}

		std::string parseNumber(LAReader& reader) {
			std::string number;

			char numberStart = reader.mustRead(NUMBER_START, "the start of a number");
			number.push_back(numberStart);

			char c = reader.mayRead(NUMBER_CONTINUED);
			while (c != '\0') {
				number.push_back(c);
				c = reader.mayRead(NUMBER_CONTINUED);
			}

			return number;
		} 
	} 

	TEST_CASE("Empty filename returns an empty list") {
		const auto [result, tokens] = tokenize("");

		CHECK(result == "");
		CHECK(tokens.size() == 0);
	}

	TEST_CASE("Empty input string returns an empty list") {
		std::istringstream input("");
		const auto[result, tokens] = tokenize(input);

		CHECK(result == "");
		CHECK(tokens.size() == 0);
	}

	TEST_CASE("Parse comments") {
		std::istringstream input("#Comment 1\n#   Comment 2\n#Comment 3");
		const auto[result, tokens] = tokenize(input);

		CHECK(result == "");
		REQUIRE(tokens.size() == 3);
		CHECK(tokens[0]->tokenType == TokenType::Comment);
		CHECK(static_cast<Comment*>(tokens[0].get())->comment == "Comment 1");
		CHECK(static_cast<Comment*>(tokens[1].get())->comment == "Comment 2");
		CHECK(static_cast<Comment*>(tokens[2].get())->comment == "Comment 3");
	}

	TEST_CASE("Parse unknown line") {
		std::istringstream input("#Comment 1\n? What is this\n#Comment 2");
		const auto[result, tokens] = tokenize(input);

		CHECK(result == "");
		REQUIRE(tokens.size() == 2);
		CHECK(tokens[0]->tokenType == TokenType::Comment);
		CHECK(static_cast<Comment*>(tokens[0].get())->comment == "Comment 1");
		CHECK(static_cast<Comment*>(tokens[1].get())->comment == "Comment 2");
	}

	TEST_CASE("Parse number") {
		std::istringstream input("-123.4abcd");
		LAReader laReader{ input };

		CHECK(parseNumber(laReader) == "-123.4");
	}

	TEST_CASE("Parse Vertices") {
		std::istringstream input("v 1 2 3\n#Some comment\n   v 4.7 -5.4 +6");
		const auto[result, tokens] = tokenize(input);

		CHECK(result == "");
		REQUIRE(tokens.size() == 3);

		CHECK(tokens[0]->tokenType == TokenType::Vertex);
		CHECK(static_cast<Vertex*>(tokens[0].get())->x == "1");
		CHECK(static_cast<Vertex*>(tokens[0].get())->y == "2");
		CHECK(static_cast<Vertex*>(tokens[0].get())->z == "3");

		CHECK(tokens[2]->tokenType == TokenType::Vertex);
		CHECK(static_cast<Vertex*>(tokens[2].get())->x == "4.7");
		CHECK(static_cast<Vertex*>(tokens[2].get())->y == "-5.4");
		CHECK(static_cast<Vertex*>(tokens[2].get())->z == "+6");
	}

	TEST_CASE("Parse Vertex Error") {
		std::istringstream input("v 1 a 3");
		const auto[result, tokens] = tokenize(input);

		CHECK(result == "Found character a, expected the start of a number");
		REQUIRE(tokens.size() == 0);
	}

}
