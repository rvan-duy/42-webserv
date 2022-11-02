#define CONFIG_CATCH_MAIN
#include "catch.hpp"
#include <string>
#include <Parser.hpp>
#include <Lexer.hpp>

std::vector<Server>	servers;
int configTest = 0;

/**
 * Test init & destruct functions
*/
static int openFile(std::ifstream &file, std::string path)
{
	file.open(path);
	if (!file.is_open())
	{
		WARN("Error opening file");
		return 1;
	}
	return 0;
}

// Used to generate file urls with testcase added to the back
// Example: testconfigs/key_value_[testnumber]
static std::string	getFileDest(std::string &baseUrl, int currentTest) {
	std::string fullDest(baseUrl);
	return fullDest.append(std::to_string(currentTest));
}

void initConfigTest(std::string baseUrl, int i) {
	// Open file
	std::ifstream 	file;
	std::string fullUrl = getFileDest(baseUrl, i);
	if (openFile(file, fullUrl)) {
		return;
	}
	// Lex file
	std::vector<Token> tokens = Lexer::tokenizeFile(file);
	file.close();
	Parser::parseTokens(&servers, tokens);
}

void	destructConfig() {
	servers.clear();
	configTest += 1;
}

/**
 * Matching functions
*/

/**************************************************/
/* TESTS				                           */
/**************************************************/

SCENARIO("Empty files 'n shit") {
	std::string	fileDest("testconfigs/empty_");
	configTest = 0;
	// empty_0
	WHEN(getFileDest(fileDest, configTest)) {
		initConfigTest(fileDest, configTest);
		REQUIRE(servers.size() == 0);
		destructConfig();
	}

	// WHEN(getFileDest(fileDest, i)) {
	// 	initConfigTest(fileDest, i);

	// 	destructConfig();
	// }
}
