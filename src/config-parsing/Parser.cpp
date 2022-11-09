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
	Server test = pServers->at(0);
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

	block.name = parseUntilSpecialChar();
	while (_it != _tokens.end()) {
		currentType = lookAhead(_it, _tokens.end());

		if (currentType == Token::WORD) {
			return 1;
		} 
		else if (currentType == Token::SEMICOLON) {
			block.dataLines.push_back(parseUntilSpecialChar());
		} 
		else if (currentType == Token::OPEN_CURL) {
			if (parseDataBlock(&block.blocks)) {
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
			_tree.dataLines.push_back(parseUntilSpecialChar());
		} else if (currentType == Token::OPEN_CURL) {
			if (parseDataBlock(&_tree.blocks)) {
				return 1;
			}
		} else {
			return 1;
		}
	}
	return 0;
}


/**************************************************/
/* Parsing abstract syntax tree			          */
/**************************************************/

void	Parser::parseDataLines(Server *pServer, std::vector<t_dataLine> const& lines) {
	for (size_t i = 0; i < lines.size(); i++) {
		for (size_t j = 0; j < PARSER_FUNC_N; j++) {
			if (lines[i].at(0) == lineParsingFuncs[j].key) {
				(this->*(lineParsingFuncs[j].func))(pServer, lines[i]);
			}
		}
	}
}

void	Parser::parseDataBlocks(Server *pServer, std::vector<DataBlock> const& blocks) {
	Route	route;
	for (size_t i = 0; i < blocks.size(); i++) {
		if (blocks[i].name.size() != 2 || blocks[i].name.at(0) != "location") {
			continue;
		}
		route = Route();
		route.route = blocks[i].name.at(1);
		for (size_t j = 0; j < BLOCK_FUNC_N; j++) {

		}
		pServer->addRoute(route);
	}
}

/* Converts datablock into server class */
Server	Parser::convertBlockToServer(DataBlock block) {
	Server	server;

	parseDataLines(&server, block.dataLines);
	parseDataBlocks(&server, block.blocks);
	return server;
}

void	Parser::parseAst(std::vector<Server>* pServers) {
	// TODO: replace with iterator
	for (size_t i = 0; i < _tree.blocks.size(); i++) {
		/* If datablock.name == "server" */
		if (_tree.blocks.at(i).name.size() == 1 &&
			_tree.blocks.at(i).name[0] == "server") {
				/* Convert datablock into server class */
				pServers->push_back(convertBlockToServer(_tree.blocks.at(i)));
			}
	}
}

/**************************************************/
/* Parsing abstract syntax tree lines	          */
/**************************************************/

t_comp Parser::lineParsingFuncs[PARSER_FUNC_N] = {
	{"listen", &Parser::parsePort},
	{"serverName", &Parser::parseServerName},
	{"errorPage", &Parser::parseErrorPage},
	{"maxBodySize", &Parser::parseMaxBodySize},
	{"return", &Parser::parseHost},
};

void	Parser::parseServerName(void *dest, t_dataLine line) {
	if (!dest || line.size() < 2) {
		return ;
	}
	Server *server = static_cast<Server *>(dest);
	std::vector<std::string> serverName;
	std::vector<std::string>::iterator it = line.begin();
	it++;
	while (it != line.end()) {
		serverName.push_back(*it);
		it++;
	}
	server->setServerName(serverName);
}

void	Parser::parseErrorPage(void *dest, t_dataLine line) {
	if (!dest || line.size() != 3) {
		return ;
	}
	Server *server = static_cast<Server *>(dest);
	std::string statusCode;

	statusCode = line.at(1);
	for (size_t i = 0; i < statusCode.length(); i++) {
		if (!isdigit(statusCode[i])) {
			return ;
		}
	}
	server->setErrorPage(std::stoi(statusCode), line.at(2));
}

void	Parser::parseHost(void *dest, t_dataLine line) {
	if (!dest || line.size() != 3) {
		return ;
	}
	Server *server = static_cast<Server *>(dest);
	std::string statusCode;

	statusCode = line.at(1);
	for (size_t i = 0; i < statusCode.length(); i++) {
		if (!isdigit(statusCode[i])) {
			return ;
		}
	}
	server->setHost(std::stoi(statusCode), line.at(2));
}

void	Parser::parsePort(void *dest, t_dataLine line) {
	if (!dest || line.size() != 2) {
		return ;
	}
	Server *server = static_cast<Server *>(dest);
	std::string	port;

	port = line.at(1);
	for (size_t i = 0; i < port.length(); i++) {
		if (!isdigit(port[i])) {
			return ;
		}
	}
	server->setPort(std::stoi(line.at(1)));
}

void	Parser::parseMaxBodySize(void *dest, t_dataLine line) {
	if (!dest || line.size() != 2) {
		return ;
	}
	Server *server = static_cast<Server *>(dest);
	std::string maxBodySize;

	maxBodySize = line.at(1);
	for (size_t i = 0; i < maxBodySize.length(); i++) {
		if (!isdigit(maxBodySize[i])) {
			return ;
		}
	}
	server->setMaxBody(std::stoi(line.at(1)));
}

/**************************************************/
/* Parsing abstract syntax tree blocks	          */
/**************************************************/

t_comp Parser::blockParsingFuncs[BLOCK_FUNC_N] = {
	{"root", &Parser::parseRoot},
	{"index", &Parser::parsePort},
	{"autoIndex", &Parser::parsePort},
	{"cgi_param", &Parser::parseCgiParam},
};

void	Parser::parseRoot(void *dest, t_dataLine line) {
	if (!dest || line.size() != 2) {
		return ;
	}
	Route *route = static_cast<Route *>(dest);
	route->searchDirectory = line.at(2);
}

void	Parser::parseIndex(void *dest, t_dataLine line) {
	if (!dest || line.size() != 2) {
		return ;
	}
	// Route *route = static_cast<Route *>(dest);
	// TODO
}

void	Parser::parseAutoIndex(void *dest, t_dataLine line) {
	if (!dest || line.size() != 2) {
		return ;
	}
	Route *route = static_cast<Route *>(dest);
	if (line.at(2) == "on") {
		route->autoIndex = true;
	} else if (line.at(2) == "off") {
		route->autoIndex = false;
	} else {
		// TODO
	}
}

void	Parser::parseCgiParam(void *dest, t_dataLine line) {
	if (!dest || line.size() != 2) {
		return ;
	}
	Route *route = static_cast<Route *>(dest);
	route->cgiParam = line.at(2);
}
