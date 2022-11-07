#pragma once
#include <Token.hpp>
#include <Server.hpp>
#include <string>
#include <vector>
#include <map>
#include <Logger.hpp>

struct DataBlock {
	// Name of datablock (positioned before the open_curl)
	std::vector<std::string>				_name;
	// Nested blocks with brackets
	std::vector<DataBlock>					_blocks;
	// Semi-colon ended lines in config file
	std::vector<std::vector<std::string> >	_dataLines;
};

struct AbstractSyntaxTree {
	// Server blocks opened and closed with curls
	std::vector<DataBlock>					_blocks;	
	// Random lines with config info
	std::vector<std::vector<std::string> >	_dataLines;
};

class Parser {
	public:
		Parser(std::vector<Token>& tokens);
		~Parser();
		int	parseTokens();

	private:
		Token::ETokenType			lookAhead(std::vector<Token>::iterator it, std::vector<Token>::iterator const& end);
		std::vector<std::string>	parseUntilSpecialChar();
		int							parseDataBlock(std::vector<DataBlock> *pDest);
		int							makeAst();


		int		parseAst();


		std::vector<Token>				_tokens;
		AbstractSyntaxTree				_tree;
		std::vector<Token>::iterator	_it;
};
