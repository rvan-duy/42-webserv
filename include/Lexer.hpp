#include <string.h>
#include <Token.hpp>
#include <vector>

class Lexer {
	public:
		static std::vector<Token> tokenizeFile(std::ifstream &file);

	private:
		// Not meant to be initialized
		Lexer() {};
		~Lexer() {};
		
		// Methods
		static ETokenType	getType(int c);
		static void	parseLine(std::vector<Token> tokens, std::string &line);
		static std::string	parseWord(std::string::iterator it);
		static bool	isSpecialChar(int c);
};
