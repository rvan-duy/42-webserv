#include <Parser.hpp>

/**
 * Constructors / destructors
*/
Parser::Parser() {}

Parser::~Parser() {}

/**
 * Parsing functions
*/
int		Parser::parseTokens(std::vector<Server> *servers, std::vector<Token>& tokens) {
	std::vector<Token>::iterator it = tokens.begin();

	while (it != tokens.end()) {
		// Server block always starts with server word
		if (!it->isWordEqualTo("server")) {
			return 1;
		}
		it++;
		// Followed by OPEN_CURL
		if (!it->isType(Token::OPEN_CURL)) {
			return 1;
		}
		it++;
		// If this is correct, start parsing server configuration
		parseServerData(servers, &it, tokens.end());
	}
	return 0;
}

int		Parser::parseServerData(std::vector<Server> *servers, std::vector<Token>::iterator *it, std::vector<Token>::iterator const& end) {
	Server				newServer;
	Token::ETokenType	currentType;

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
				break;
			default:
				parseServerWord(&newServer, it, end);
		}
	}
	servers->push_back(newServer);
	return 0;
}

int		Parser::parseServerWord(Server *server, std::vector<Token>::iterator *it, std::vector<Token>::iterator const& end) {

}
