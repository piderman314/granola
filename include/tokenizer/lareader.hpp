#ifndef TOKENIZER_LAREADER_HPP
#define TOKENIZER_LAREADER_HPP

#include "token.hpp"

#include <functional>
#include <istream>
#include <vector>

namespace granola::tokenizer {

	using CharPredicate = std::function<bool(char)>;

	class LAReader {
	private:
		std::istreambuf_iterator<char> chars;
		char la;

	public:
		LAReader(std::istream& stream);

		bool mayBe(char c) const noexcept;
		bool mayRead(char c) noexcept;
		void mustRead(char c);
		bool atEos() const noexcept;
		std::string readLine() noexcept;
		void skipWhitespace() noexcept;

	private:
		void lookAhead() noexcept;
		void skipUntil(CharPredicate pred);
		std::string readUntil(CharPredicate pred);

	};

	class LAReaderException : std::exception {

	public:
		std::string message;

		LAReaderException(std::string message) : message{ message } {}
	};

}

#endif // TOKENIZER_LAREADER_HPP