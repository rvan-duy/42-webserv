#include <Parser.hpp>

/**************************************************/
/* General parser functions			 	          */
/**************************************************/
Parser::Parser(std::vector<Token>& tokens): _tokens(tokens), _it(_tokens.begin()) {}

Parser::~Parser() {}

int		Parser::parseTokens() {
	if (makeAst()) {
		return 1;
	}
	// while (_it != _tokens.end()) {
	// 	parseDataBlock(&blocks);
	// 	_it++;
	// }
	return 0;
}

/**************************************************/
/* Making abstract syntax tree		 	          */
/**************************************************/
/**
 * Looks ahead in tokens to see what kind of parsing is needed
 * Different parsing method is used for open curl or semicolon
 * 
 * !!! Returns WORD in case no special char is found
 * 	This is because both a WORD and no special char ends up in errors
*/
Token::ETokenType	Parser::lookAhead(std::vector<Token>::iterator it, std::vector<Token>::iterator const& end) {
	while (it != end && it->isType(Token::WORD)) {
		it++;
	}
	// TODO: Better error handling
	if (it == end) {
		return Token::WORD;
	}
	return it->getType();
}

/**
 * Iterates over tokens until it encounters a special char
 * Then returns all words from tokens it parsed
 * !!! Doesn't do any error or end of vector checking
*/
std::vector<std::string>	Parser::parseUntilSpecialChar() {
	std::vector<std::string>	line;

	while (_it->isType(Token::WORD)) {
		line.push_back(_it->getWord());
		_it++;
	}
	_it++;
	return line;
}

/**
 * A data block is a block that is opened and closed by curls
*/
int		Parser::parseDataBlock(std::vector<DataBlock> *pDest) {
	Token::ETokenType currentType;
	DataBlock	block;

	block._name = parseUntilSpecialChar();
	while (_it != _tokens.end()) {
		currentType = lookAhead(_it, _tokens.end());

		if (currentType == Token::WORD) {
			return 1;
		} 
		else if (currentType == Token::SEMICOLON) {
			block._dataLines.push_back(parseUntilSpecialChar());
		} 
		else if (currentType == Token::OPEN_CURL) {
			if (parseDataBlock(&block._blocks)) {
				return 1;
			}
		}
		else if (currentType == Token::CLOSE_CURL) {
			pDest->push_back(block);
			_it++;
			return 0;
		}
	}
	return 1;
}

int	Parser::makeAst() {
	Token::ETokenType currentType;

	while (_it != _tokens.end()) {
		currentType = lookAhead(_it, _tokens.end());
		if (currentType == Token::SEMICOLON) {
			_tree._dataLines.push_back(parseUntilSpecialChar());
		} else if (currentType == Token::OPEN_CURL) {
			if (parseDataBlock(&_tree._blocks)) {
				return 1;
			}
		} else {
			return 1;
		}
	}
	return 0;
}

/**************************************************/
/* Parsing abstract syntax tree		 	          */
/**************************************************/

int	Parser::parseAst() {
	return 0;
}
