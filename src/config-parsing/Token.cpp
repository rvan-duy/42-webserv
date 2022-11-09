#include <Token.hpp>
#include <Logger.hpp>

/**
 * Constructors / destructors
*/
Token::Token(std::string word): _word(word), _type(Token::WORD)  {}

Token::Token(Token::ETokenType type): _word(""), _type(type) {}

Token::~Token() {}

/**
 * Getters
*/
Token::ETokenType	&Token::getType() {
	return _type;
}

std::string &Token::getWord() {
	if (_type != WORD) {
		Logger::getInstance().error("Attempted to get word from non-WORD token");
		// TODO: throw error??
		return _word;
	}
	return _word;
}

/**
 * Cool stuff
*/
bool	Token::isWordEqualTo(std::string const& value) const {
	if (_type != WORD) {
		return false;
	}
	return (value == _word);
}

bool	Token::isType(Token::ETokenType const& type) const {
	return (_type == type);
}
