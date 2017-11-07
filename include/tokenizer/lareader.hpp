#ifndef TOKENIZER_LAREADER_HPP
#define TOKENIZER_LAREADER_HPP

#include "token.hpp"

#include <istream>
#include <vector>

namespace granola::tokenizer {

	class LAReader {
	private:
		std::istreambuf_iterator<char> chars;

	public:
		LAReader(std::istream& stream);
	};

}

#endif // TOKENIZER_LAREADER_HPP