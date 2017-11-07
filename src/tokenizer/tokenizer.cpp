#include "tokenizer/tokenizer.hpp"

#include "tokenizer/lareader.hpp"

#include <fstream>
#include <istream>
#include <doctest.h>

#ifndef DOCTEST_CONFIG_DISABLE
#include <sstream>
#endif

namespace granola::tokenizer {

	auto tokenize(std::istream& stream) {
		LAReader reader{ stream };

		std::vector<Token> tokens;

		return tokens;
	}

	auto tokenize(const std::string& file) {
		if (file == "") {
			return std::vector<Token>();
		}

		std::ifstream file_input{ file };
		return tokenize(file_input);
	}

	TEST_CASE("Empty filename returns an empty list") {
		auto tokens = tokenize("");

		CHECK(tokens.size() == 0);
	}

	TEST_CASE("Empty input string returns an empty list") {
		std::istringstream input("");
		auto tokens = tokenize(input);

		CHECK(tokens.size() == 0);
	}

}
