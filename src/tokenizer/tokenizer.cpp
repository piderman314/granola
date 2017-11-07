#include "tokenizer/tokenizer.hpp"

#include <fstream>
#include <doctest.h>

namespace granola::tokenizer {

	auto tokenize(const std::string& file) {
		std::vector<Token> tokens;

		if (file == "") {
			return tokens;
		}

		return tokens;
	}

	TEST_CASE("Empty filename returns an empty list") {
		auto tokens = tokenize("");

		CHECK(tokens.size() == 0);
	}

}
