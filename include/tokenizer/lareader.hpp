#ifndef TOKENIZER_LAREADER_HPP
#define TOKENIZER_LAREADER_HPP

#include "token.hpp"

#include <istream>
#include <vector>

namespace granola::tokenizer {

	class LAReader {
	private:
		std::istreambuf_iterator<char> chars;
		char la;

	public:
		LAReader(std::istream& stream);

		bool mayBe(char c) const noexcept;
		bool mayRead(char c) noexcept;
		void mustRead(char c);

	private:
		void lookAhead() noexcept;
		bool atEos() const noexcept;

	};

	class LAReaderException : std::exception {

	public:
		std::string message;

		LAReaderException(std::string message) : message{ message } {}
	};

}

#endif // TOKENIZER_LAREADER_HPP