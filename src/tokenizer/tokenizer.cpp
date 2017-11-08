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

		std::vector<Token> tokens;

		return std::make_tuple("", tokens);
	}

	TokenizeResult tokenize(const std::string& file) {
		if (file == "") {
			return std::make_tuple("", std::vector<Token>());
		}

		std::ifstream file_input{ file };
		return tokenize(file_input);
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

}
