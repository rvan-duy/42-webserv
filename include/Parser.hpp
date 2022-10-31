#include <Token.hpp>
#include <Server.hpp>
#include <string>
#include <vector>

class Parser {
	public:
		static int	parseTokens(std::vector<Server> *servers, std::vector<Token>& tokens);

	private:
		Parser();
		~Parser();

		static int	parseServerWord(Server *server, std::vector<Token>::iterator *it, std::vector<Token>::iterator const& end);
		static int	parseServerData(std::vector<Server> *servers, std::vector<Token>::iterator *it, std::vector<Token>::iterator const& end);
};
