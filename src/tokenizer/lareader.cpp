#include "tokenizer/lareader.hpp"

#include <doctest.h>

#ifndef DOCTEST_CONFIG_DISABLE
#include <sstream>
#endif

namespace granola::tokenizer {

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

	void LAReader::lookAhead() noexcept {
		if (!atEos()) {
			la = *chars;
			chars++;
		}
	}

	namespace {
		std::istreambuf_iterator<char> EOS;
	}

	bool LAReader::atEos() const noexcept {
		return chars == EOS;
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
				CHECK_MESSAGE(false, "Should not have thrown an exception");
			}

			try {
				laReader.mustRead('z');
				CHECK_MESSAGE(false, "Should have thrown an exception");
			} catch (const LAReaderException& re) {
				CHECK(re.message == "Found character b, expected z");
			}
		}
	}

}