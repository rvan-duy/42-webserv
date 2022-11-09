#pragma once
#include <Token.hpp>
#include <array>
#include <Server.hpp>
#include <string>
#include <vector>
#include <map>
#include <Logger.hpp>

/* Circle inclusion, so doesn't compile without this parser definition */
class Parser;
/* Parser member function typedef */
typedef void	(Parser::*ParserFunc)(Server *dest, std::vector<std::string> dataLine);

/* Key - parser function pair */
typedef struct s_comp
{
    std::string	key;
    ParserFunc 	func;
}	t_comp;

typedef std::vector<std::string> t_dataLine;

/*
	Example datablock:
	server {
		listen: 80;
	}
*/
struct DataBlock {
	// Name of datablock (positioned before the open_curl)
	std::vector<std::string>	_name;
	// Nested blocks with brackets
	std::vector<DataBlock>		_blocks;
	// Semi-colon ended lines in config file
	std::vector<t_dataLine>		_dataLines;
};

struct AbstractSyntaxTree {
	// Server blocks opened and closed with curls
	std::vector<DataBlock>		_blocks;	
	// Random lines with config info
	std::vector<t_dataLine>		_dataLines;
};

/**
 * Takes a vector of tokens and parses them into a vector of servers
*/
class Parser {
	public:
		Parser(std::vector<Token>& tokens);
		~Parser();
		int	parseTokens(std::vector<Server>* pServers);

	private:
		Token::ETokenType			lookAhead(std::vector<Token>::iterator it, std::vector<Token>::iterator const& end);
		std::vector<std::string>	parseUntilSpecialChar();
		int							parseDataBlock(std::vector<DataBlock> *pDest);
		int							makeAst();

		void	parseAst(std::vector<Server>* pServers);
		Server	convertBlockToServer(DataBlock block);

		/* AST parsing functions */
		void	parsePort(Server *dest, t_dataLine line);
		void	parseMaxBodySize(Server *dest, t_dataLine line);
		void	parseErrorPage(Server *dest, t_dataLine line);
		void	parseHost(Server *dest, t_dataLine line);
		void	parseServerName(Server *dest, t_dataLine line);
		/* table with key value pairs for parsing AST */
		static t_comp parserFuncTable[];

		std::vector<Token>				_tokens;
		AbstractSyntaxTree				_tree;
		std::vector<Token>::iterator	_it;

};
