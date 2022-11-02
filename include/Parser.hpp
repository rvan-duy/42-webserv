#pragma once
#include <Token.hpp>
#include <Server.hpp>
#include <string>
#include <vector>
#include <Logger.hpp>

class Parser {
	public:
		Parser(std::vector<Token> tokens);
		~Parser();
		static int	parseTokens(std::vector<Server> *servers, std::vector<Token>& tokens);

	private:
		std::vector<Token>				_tokens;
		std::vector<Token>::iterator	_it;
		std::vector<Server>				_servers;
		
		static int	parseServerWord(Server *server, std::vector<Token>::iterator *it, std::vector<Token>::iterator const& end);
		static int	parseServerData(std::vector<Server> *servers, std::vector<Token>::iterator *it, std::vector<Token>::iterator const& end);
};
