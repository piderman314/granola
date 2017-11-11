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
		explicit LAReader(std::istream& stream) noexcept;

		bool mayBe(CharPredicate pred) const noexcept;
		bool mayBe(char c) const noexcept;
		char mayRead(CharPredicate pred) noexcept;
		bool mayRead(char c) noexcept;
		char mustRead(CharPredicate pred, std::string expected); // throws LAReaderException
		void mustRead(char c); // throws LAReaderException
		bool atEos() const noexcept;
		void skipLine() noexcept;
		std::string readLine() noexcept;
		void skipWhitespace() noexcept;

	private:
		void lookAhead() noexcept;
		void skipUntil(CharPredicate pred) noexcept;
		std::string readUntil(CharPredicate pred) noexcept;

	};

	class LAReaderException : public std::exception {

	public:
		std::string message;

		explicit LAReaderException(const std::string& message) : message{ message } {}
	};

}

#endif // TOKENIZER_LAREADER_HPP