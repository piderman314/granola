#include "tokenizer/lareader.hpp"

namespace granola::tokenizer {

	LAReader::LAReader(std::istream& stream) : chars{ stream } {}

}