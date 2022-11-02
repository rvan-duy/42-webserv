#include <Parser.hpp>

/**
 * Constructors / destructors
*/
Parser::Parser(std::vector<Token> tokens): _tokens(tokens), _it(tokens.begin()) {}

Parser::~Parser() {}

/**
 * Parsing functions
*/
int		Parser::parseTokens(std::vector<Server> *servers) {
	Logger& logger = Logger::getInstance();
	if (servers == NULL) {
		logger.error("Incorrect input in token parser");
		return 1;
	}
	logger.log("Starting parsing tokens");

	while (_it != tokens.end()) {
		// Server block always starts with server word
		if (!_it->isWordEqualTo("server")) {
			servers->clear();
			return 1;
		}
		_it++;
		// Followed by OPEN_CURL
		if (!_it->isType(Token::OPEN_CURL)) {
			servers->clear();
			return 1;
		}
		_it++;
		// If this is correct, start parsing server configuration
		if (parseServerData(servers, tokens.end()) == 1) {
			servers->clear();
			return 1;
		}
	}
	return 0;
}

int		Parser::parseServerData(std::vector<Server> *servers, std::vector<Token>::iterator *it, std::vector<Token>::iterator const& end) {
	Token::ETokenType	currentType;
	Server				newServer;

	while (const_cast<std::vector<Token>::iterator&>(*it) != end) {
		currentType = (*it)->getType();
		switch (currentType) {
			case Token::OPEN_CURL:
				// Can't have another open curl
				return 1;
			case Token::SEMICOLON:
				// Skip random semicolons
				*it += 1;
				continue;
			case Token::CLOSE_CURL:
				// Ends server block
				*it += 1;
				servers->push_back(newServer);
				return 0;
			default:
				parseServerWord(&newServer, it, end);
		}
	}
	return 1;
}

int		Parser::parseServerWord(Server *server, std::vector<Token>::iterator *it, std::vector<Token>::iterator const& end) {
	while (const_cast<std::vector<Token>::iterator&>(*it) != end) {
		
	}
}
