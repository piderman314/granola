#pragma once

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