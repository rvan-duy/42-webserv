#include <Parser.hpp>

/**************************************************/
/* General parser functions			 	          */
/**************************************************/

Parser::Parser(std::vector<Token> &tokens) : _tokens(tokens), _it(_tokens.begin()) {}

Parser::~Parser() {}

int Parser::parseTokens(std::vector<Server> *pServers)
{
	if (makeAst())
	{
		return 1;
	}
	if (parseAst(pServers))
	{
		return 1;
	}
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
Token::ETokenType Parser::lookAhead(std::vector<Token>::iterator it, std::vector<Token>::iterator const &end)
{
	while (it != end && it->isType(Token::WORD))
	{
		it++;
	}
	if (it == end)
	{
		return Token::WORD;
	}
	return it->getType();
}

/**
 * Iterates over tokens until it encounters a special char
 * Then returns all words from tokens it parsed
 * !!! Doesn't do any error or end of vector checking
 */
std::vector<std::string> Parser::parseUntilSpecialChar()
{
	std::vector<std::string> line;

	while (_it->isType(Token::WORD))
	{
		line.push_back(_it->getWord());
		_it++;
	}
	_it++;
	return line;
}

/**
 * A data block is a block that is opened and closed by curls
 */
int Parser::parseDataBlock(std::vector<DataBlock> *pDest)
{
	Token::ETokenType currentType;
	DataBlock block;

	block.name = parseUntilSpecialChar();
	while (_it != _tokens.end())
	{
		currentType = lookAhead(_it, _tokens.end());

		if (currentType == Token::WORD)
		{
			return 1;
		}
		else if (currentType == Token::SEMICOLON)
		{
			block.dataLines.push_back(parseUntilSpecialChar());
		}
		else if (currentType == Token::OPEN_CURL)
		{
			if (parseDataBlock(&block.blocks))
			{
				return 1;
			}
		}
		else if (currentType == Token::CLOSE_CURL)
		{
			pDest->push_back(block);
			_it++;
			return 0;
		}
	}
	return 1;
}

int Parser::makeAst()
{
	Token::ETokenType currentType;

	while (_it != _tokens.end())
	{
		currentType = lookAhead(_it, _tokens.end());
		if (currentType == Token::SEMICOLON)
		{
			_tree.dataLines.push_back(parseUntilSpecialChar());
		}
		else if (currentType == Token::OPEN_CURL)
		{
			if (parseDataBlock(&_tree.blocks))
			{
				return 1;
			}
		}
		else
		{
			return 1;
		}
	}
	return 0;
}

/**************************************************/
/* Parsing abstract syntax tree			          */
/**************************************************/

/* Parses all datalines in server block */
int Parser::parseServerDataLines(Server *pServer, std::vector<t_dataLine> const &lines)
{
	for (size_t i = 0; i < lines.size(); i++)
	{
		for (size_t j = 0; j < PARSER_FUNC_N; j++)
		{
			if (lines[i].size() != 0 && lines[i].at(0) == lineParsingFuncs[j].key)
			{
				if ((this->*(lineParsingFuncs[j].func))(pServer, lines[i]))
				{
					return 1;
				}
			}
		}
	}
	return 0;
}

/* Parses location blocks in server */
int Parser::parseLocationBlocks(Route *pRoute, std::vector<t_dataLine> const &lines)
{
	for (size_t i = 0; i < lines.size(); i++)
	{
		for (size_t j = 0; j < PARSER_FUNC_N; j++)
		{
			if (lines[i].size() != 0 && lines[i].at(0) == blockParsingFuncs[j].key)
			{
				if ((this->*(blockParsingFuncs[j].func))(pRoute, lines[i]))
				{
					return 1;
				}
			}
		}
	}
	return 0;
}

static Route initNewRoute(std::string routeName)
{
	Route route = Route();

	route.allowedMethods[GET] = true;
	route.allowedMethods[POST] = true;
	route.allowedMethods[DELETE] = true;
	route.route = routeName;
	return route;
}

int Parser::parseDataBlocks(Server *pServer, std::vector<DataBlock> const &blocks)
{
	for (size_t i = 0; i < blocks.size(); i++)
	{
		if (blocks[i].name.size() != 2 || blocks[i].name.at(0) != "location")
		{
			continue;
		}
		Route route = initNewRoute(blocks[i].name.at(1));
		if (parseLocationBlocks(&route, blocks[i].dataLines))
		{
			return 1;
		}
		pServer->addRoute(route);
	}
	return 0;
}

/* Converts datablock into server class */
int Parser::convertBlockToServer(Server *pServer, DataBlock block)
{
	if (parseServerDataLines(pServer, block.dataLines))
	{
		return 1;
	}
	if (parseDataBlocks(pServer, block.blocks))
	{
		return 1;
	}
	return 0;
}

/* Loops over datablocks to find blocks that start with "server", then converts them*/
int Parser::parseAst(std::vector<Server> *pServers)
{
	Server server;
	for (size_t i = 0; i < _tree.blocks.size(); i++)
	{
		/* If datablock.name == "server" */
		if (_tree.blocks.at(i).name.size() == 1 &&
			_tree.blocks.at(i).name[0] == "server")
		{
			server = Server();
			if (convertBlockToServer(&server, _tree.blocks.at(i)))
			{
				return 1;
			}
			/* Convert datablock into server class */
			pServers->push_back(server);
		}
	}
	return 0;
}
