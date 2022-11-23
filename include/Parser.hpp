#pragma once
#include <Token.hpp>
#include <array>
#include <Server.hpp>
#include <string>
#include <vector>
#include <map>
#include <Logger.hpp>

#define PARSER_FUNC_N 5
#define BLOCK_FUNC_N 4

/* Circle inclusion, so doesn't compile without this parser definition */
class Parser;
/* Parser member function typedef */
typedef int (Parser::*ParserFunc)(void *dest, std::vector<std::string> dataLine);

/* Key - parser function pair */
typedef struct s_comp
{
	std::string key;
	ParserFunc func;
} t_comp;

typedef std::vector<std::string> t_dataLine;

/*
	Example datablock:
	server {
		listen: 80;
	}
*/
struct DataBlock
{
	// Name of datablock (positioned before the open_curl)
	std::vector<std::string> name;
	// Nested blocks with brackets
	std::vector<DataBlock> blocks;
	// Semi-colon ended lines in config file
	std::vector<t_dataLine> dataLines;
};

struct AbstractSyntaxTree
{
	// Server blocks opened and closed with curls
	std::vector<DataBlock> blocks;
	// Random lines with config info
	std::vector<t_dataLine> dataLines;
};

/**
 * Takes a vector of tokens and parses them into a vector of servers
 */
class Parser
{
public:
	Parser(std::vector<Token> &tokens);
	~Parser();
	int parseTokens(std::vector<Server> *pServers);

private:
	Token::ETokenType lookAhead(std::vector<Token>::iterator it, std::vector<Token>::iterator const &end);
	std::vector<std::string> parseUntilSpecialChar();
	int parseDataBlock(std::vector<DataBlock> *pDest);
	int makeAst();

	void parseAst(std::vector<Server> *pServers);
	void parseDataLines(Server *pServer, std::vector<t_dataLine> const &lines);
	void parseDataBlocks(Server *pServer, std::vector<DataBlock> const &blocks);
	Server convertBlockToServer(DataBlock block);

	/* AST line parsing functions */
	int parsePort(void *dest, t_dataLine line);
	int parseMaxBodySize(void *dest, t_dataLine line);
	int parseErrorPage(void *dest, t_dataLine line);
	int parseHost(void *dest, t_dataLine line);
	int parseServerName(void *dest, t_dataLine line);
	/* AST block parsing functions */
	int parseRoot(void *dest, t_dataLine line);
	int parseIndex(void *dest, t_dataLine line);
	int parseAutoIndex(void *dest, t_dataLine line);
	int parseCgiParam(void *dest, t_dataLine line);
	/* table with key value pairs for parsing AST */
	static t_comp lineParsingFuncs[PARSER_FUNC_N];
	static t_comp blockParsingFuncs[BLOCK_FUNC_N];

	std::vector<Token> _tokens;
	AbstractSyntaxTree _tree;
	std::vector<Token>::iterator _it;
};
