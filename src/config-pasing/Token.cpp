#include <Token.hpp>
#include <Logger.hpp>

Token::Token(std::string word): _word(word), _type(WORD)  {}

Token::Token(ETokenType type): _word(""), _type(type) {}

ETokenType	Token::getType() const {
	return _type;
}

std::string Token::getWord() const {
	if (_type != WORD) {
		Logger::getInstance().error("Attempted to get word from non-WORD token");
		// throw error;
	}
	return _word;
}
