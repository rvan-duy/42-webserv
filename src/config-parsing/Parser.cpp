#include <Parser.hpp>

/**************************************************/
/* General parser functions			 	          */
/**************************************************/

Parser::Parser(std::vector<Token>& tokens): _tokens(tokens), _it(_tokens.begin()) {}

Parser::~Parser() {}

int		Parser::parseTokens(std::vector<Server>* pServers) {
	/* First transform tokens into abstract syntax tree*/
	if (makeAst()) {
		return 1;
	}
	parseAst(pServers);
	return 0;
}

/**************************************************/
/* Making abstract syntax tree		 	          */
/**************************************************/
/**
 * Looks ahead in tokens to see what kind of parsing is needed
 * Different parsing method is used for open curl or semicolon
 * 
 * !!! Returns WORD in case of EOF
 * 	This is because both a WORD and EOF end up in errors
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

t_comp Parser::parserFuncTable[3] = {
	{"listen", &Parser::parsePort},
	{"serverName", &Parser::parseServerName},
	{"errorPage", &Parser::parseErrorPage},
	{"maxBodySize", &Parser::parseMaxBodySize},
	{"return", &Parser::parseHost},
};

void	Parser::parseServerName(Server *dest, t_dataLine line) {
	if (!dest || line.size() < 2) {
		return ;
	}
	std::vector<std::string> serverName(line.size() - 1);
	std::vector<std::string>::iterator it = line.begin();
	it++;
	while (it != line.end()) {
		serverName.push_back(*it);
		it++;
	}
	dest->setServerName(serverName);
}

void	Parser::parseErrorPage(Server *dest, t_dataLine line) {
	if (!dest || line.size() != 3) {
		return ;
	}
	std::string statusCode;

	statusCode = line.at(1);
	for (size_t i = 0; i < statusCode.length(); i++) {
		if (!isdigit(statusCode[i])) {
			return ;
		}
	}
	dest->setErrorPage(std::stoi(statusCode), line.at(2));
}

void	Parser::parseHost(Server *dest, t_dataLine line) {
	if (!dest || line.size() != 3) {
		return ;
	}
	std::string statusCode;

	statusCode = line.at(1);
	for (size_t i = 0; i < statusCode.length(); i++) {
		if (!isdigit(statusCode[i])) {
			return ;
		}
	}
	dest->setHost(std::stoi(statusCode), line.at(2));
}

void	Parser::parsePort(Server *dest, t_dataLine line) {
	if (!dest || line.size() != 2) {
		return ;
	}
	std::string	port;

	port = line.at(1);
	for (size_t i = 0; i < port.length(); i++) {
		if (!isdigit(port[i])) {
			return ;
		}
	}
	dest->setPort(std::stoi(line.at(1)));
}

void	Parser::parseMaxBodySize(Server *dest, t_dataLine line) {
	if (!dest || line.size() != 2) {
		return ;
	}
	std::string maxBodySize;

	maxBodySize = line.at(1);
	for (size_t i = 0; i < maxBodySize.length(); i++) {
		if (!isdigit(maxBodySize[i])) {
			return ;
		}
	}
	dest->setMaxBody(std::stoi(line.at(1)));
}

/* Converts datablock into server class */
Server	Parser::convertBlockToServer(DataBlock block) {
	std::vector<t_dataLine >::iterator it = block._dataLines.begin();
	Server	server;


	return server;
}

void	Parser::parseAst(std::vector<Server>* pServers) {
	// TODO: replace with iterator
	for (size_t i = 0; i < _tree._blocks.size(); i++) {
		/* If datablock.name == "server" */
		if (_tree._blocks.at(i)._name.size() == 1 &&
			_tree._blocks.at(i)._name[0] == "server") {
				/* Convert datablock into server class */
				convertBlockToServer(_tree._blocks.at(i));
			}
	}
}
