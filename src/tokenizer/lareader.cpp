#include "tokenizer/lareader.hpp"

#include <doctest.h>

#ifndef DOCTEST_CONFIG_DISABLE
#include <sstream>
#endif

namespace granola::tokenizer {

	namespace {
		const std::istreambuf_iterator<char> EOS;

		const char EOL_CHAR = '\n';
		const CharPredicate EOL = [](char c) { return c == EOL_CHAR; };

		const char SPACE_CHAR = ' ';
		const char TAB_CHAR = '\t';
		const CharPredicate WHITESPACE = [](char c) { return c == SPACE_CHAR || c == TAB_CHAR; };

		const char NULL_CHAR = '\0';
	}

	LAReader::LAReader(std::istream& stream) : chars{ stream } {
		lookAhead();
	}

	bool LAReader::mayBe(char c) const noexcept {
		return !atEos() && c == la;
	}

	bool LAReader::mayRead(char c) noexcept {
		if (atEos() || c != la) {
			return false;
		}

		lookAhead();

		return true;
	}

	void LAReader::mustRead(char c) {
		if (atEos()) {
			throw LAReaderException { "Unexpected end of data, expected " + std::string(1, c) };
		}

		if (c != la) {
			throw LAReaderException { "Found character " + std::string(1, la) + ", expected " + std::string(1, c) };
		}

		lookAhead();
	}

	bool LAReader::atEos() const noexcept {
		return chars == EOS && la == NULL_CHAR;
	}

	std::string LAReader::readLine() noexcept {
		std::string line = readUntil(EOL);

		mayRead(EOL_CHAR); // or EOS

		return line;
	}

	void LAReader::skipWhitespace() noexcept {
		skipUntil(std::not1(WHITESPACE));
	}

	void LAReader::lookAhead() noexcept {
		if (chars != EOS) {
			la = *chars;
			chars++;
		} else {
			if (la != NULL_CHAR) {
				la = NULL_CHAR;
			}
		}
	}

	void LAReader::skipUntil(CharPredicate pred) {
		while (!atEos() && !pred(la)) {
			lookAhead();
		}
	}

	std::string LAReader::readUntil(CharPredicate pred) {
		std::string result;

		while (!atEos() && !pred(la)) {
			result.push_back(la);
			lookAhead();
		}

		return result;
	}
	
	TEST_CASE("LAReader at EOS") {
		std::istringstream input("");
		LAReader laReader{ input };

		SUBCASE("mayBe") {
			CHECK(laReader.mayBe('a') == false);
		}

		SUBCASE("mayRead") {
			CHECK(laReader.mayRead('a') == false);
		}

		SUBCASE("mustRead") {
			try {
				laReader.mustRead('a');
				CHECK_MESSAGE(false, "Should have thrown an exception");
			} catch (const LAReaderException& re) {
				CHECK(re.message == "Unexpected end of data, expected a");
			}
		}
	}

	TEST_CASE("LAReader with text") {
		std::istringstream input("abcdef");
		LAReader laReader{ input };

		SUBCASE("mayBe") {
			CHECK(laReader.mayBe('a') == true);
			CHECK(laReader.mayBe('b') == false);
		}

		SUBCASE("mayRead") {
			CHECK(laReader.mayRead('a') == true);
			CHECK(laReader.mayRead('b') == true);
			CHECK(laReader.mayRead('z') == false);
		}

		SUBCASE("mustRead") {
			try {
				laReader.mustRead('a');
			} catch (const LAReaderException& re) {
				CHECK_MESSAGE(false, re.message);
			}

			try {
				laReader.mustRead('z');
				CHECK_MESSAGE(false, "Should have thrown an exception");
			} catch (const LAReaderException& re) {
				CHECK(re.message == "Found character b, expected z");
			}
		}
	}

	TEST_CASE("LAReader readLine") {
		std::istringstream input{ "Line 1\nLine 2\nLine 3" };
		LAReader laReader{ input };

		CHECK(laReader.readLine() == "Line 1");
		CHECK(laReader.readLine() == "Line 2");
		CHECK(laReader.readLine() == "Line 3");
	}

	TEST_CASE("LAReader skipWhitespace") {
		std::istringstream input{ "a  \t\t  b" };
		LAReader laReader{ input };

		try {
			laReader.mustRead('a');
			laReader.skipWhitespace();
			laReader.mustRead('b');
		} catch (const LAReaderException& re) {
			CHECK_MESSAGE(false, re.message);
		}
	}

}