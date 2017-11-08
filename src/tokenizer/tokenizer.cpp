#include "tokenizer/tokenizer.hpp"

#include "tokenizer/lareader.hpp"

#include <fstream>
#include <istream>
#include <doctest.h>

#ifndef DOCTEST_CONFIG_DISABLE
#include <sstream>
#endif

namespace granola::tokenizer {

	TokenizeResult tokenize(std::istream& stream) {
		LAReader reader{ stream };

		std::vector<std::unique_ptr<Token>> tokens;

		try {
			while (!reader.atEos()) {
				if (reader.mayBe('#')) {
					tokens.push_back(parseComment(reader));
				}
			}
		} catch (const LAReaderException& re) {
			return std::make_tuple(re.message, std::move(tokens));
		}

		return std::make_tuple(std::string(""), std::move(tokens));
	}

	TokenizeResult tokenize(const std::string& file) {
		if (file == "") {
			return std::make_tuple("", std::vector<std::unique_ptr<Token>>());
		}

		std::ifstream file_input{ file };
		return tokenize(file_input);
	}

	namespace {
		std::unique_ptr<Comment> parseComment(LAReader& reader) {
			reader.mustRead('#');
			reader.skipWhitespace();

			return std::make_unique<Comment>(reader.readLine());;
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

}
