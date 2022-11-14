#define CONFIG_CATCH_MAIN
#include "catch.hpp"
#include <string>
#include <Parser.hpp>
#include <Lexer.hpp>

std::vector<Server>	servers;

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

static int initConfigTest(std::string baseUrl, int i) {
	// Open file
	std::ifstream 	file;
	std::string fullUrl = getFileDest(baseUrl, i);
	if (openFile(file, fullUrl)) {
		return 1;
	}
	// Lex file
	std::vector<Token> tokens = Lexer::tokenizeFile(file);
	file.close();
	Parser	parser(tokens);
	return parser.parseTokens(&servers);
}

static void	destructConfig() {
	servers.clear();
}

/**************************************************/
/* TESTS				                           */
/**************************************************/

SCENARIO("Empty files 'n shit") {
	std::string	fileDest("testconfigs/empty_");
	int configTest = 0;

	// empty_0
	WHEN(getFileDest(fileDest, configTest)) {
		int ret = initConfigTest(fileDest, configTest);
		REQUIRE(ret == 0);
		REQUIRE(servers.size() == 0);
		destructConfig();
	}
	configTest++;

	// empty_1
	WHEN(getFileDest(fileDest, configTest)) {
		int ret = initConfigTest(fileDest, configTest);
		REQUIRE(ret == 0);
		REQUIRE(servers.size() == 0);
		destructConfig();
	}
	configTest++;

	// empty_2
	WHEN(getFileDest(fileDest, configTest)) {
		int ret = initConfigTest(fileDest, configTest);
		REQUIRE(ret == 1);
		REQUIRE(servers.size() == 0);
		destructConfig();
	}
	configTest++;

	// empty_3
	WHEN(getFileDest(fileDest, configTest)) {
		int ret = initConfigTest(fileDest, configTest);
		REQUIRE(ret == 0);
		REQUIRE(servers.size() == 0);
		destructConfig();
	}
	configTest++;

	// empty_4
	WHEN(getFileDest(fileDest, configTest)) {
		int ret = initConfigTest(fileDest, configTest);
		REQUIRE(ret == 0);
		REQUIRE(servers.size() == 0);
		destructConfig();
	}
	configTest++;
}

SCENARIO("Bad config files") {
	std::string	fileDest("testconfigs/bad_");
	int configTest = 0;
	// bad_0
	WHEN(getFileDest(fileDest, configTest)) {
		int ret = initConfigTest(fileDest, configTest);
		REQUIRE(ret == 1);
		REQUIRE(servers.size() == 0);
		destructConfig();
	}
	configTest++;

	// bad_1
	WHEN(getFileDest(fileDest, configTest)) {
		int ret = initConfigTest(fileDest, configTest);
		REQUIRE(ret == 1);
		REQUIRE(servers.size() == 0);
		destructConfig();
	}
	configTest++;

	// bad_2
	WHEN(getFileDest(fileDest, configTest)) {
		int ret = initConfigTest(fileDest, configTest);
		REQUIRE(ret == 1);
		REQUIRE(servers.size() == 0);
		destructConfig();
	}
	configTest++;

	// bad_3
	WHEN(getFileDest(fileDest, configTest)) {
		int ret = initConfigTest(fileDest, configTest);
		REQUIRE(ret == 1);
		REQUIRE(servers.size() == 0);
		destructConfig();
	}
	configTest++;

	// bad_4
	WHEN(getFileDest(fileDest, configTest)) {
		int ret = initConfigTest(fileDest, configTest);
		REQUIRE(ret == 1);
		REQUIRE(servers.size() == 0);
		destructConfig();
	}
	configTest++;

	// bad_5
	WHEN(getFileDest(fileDest, configTest)) {
		int ret = initConfigTest(fileDest, configTest);
		REQUIRE(ret == 1);
		REQUIRE(servers.size() == 0);
		destructConfig();
	}
	configTest++;
}

SCENARIO("Test config files") {
	std::string	fileDest("testconfigs/good_");
	int configTest = 0;
	// parser_0
	WHEN(getFileDest(fileDest, configTest)) {
		int ret = initConfigTest(fileDest, configTest);
		REQUIRE(ret == 0);
		REQUIRE(servers.size() == 1);
		REQUIRE_FALSE(servers[0].hasServerName());
		REQUIRE_FALSE(servers[0].hasMaxBody());
		REQUIRE_FALSE(servers[0].hasRoutes());
		destructConfig();
	}
	configTest++;

	// parser_1
	WHEN(getFileDest(fileDest, configTest)) {
		int ret = initConfigTest(fileDest, configTest);
		REQUIRE(ret == 0);
		REQUIRE(servers.size() == 2);
		REQUIRE_FALSE(servers[0].hasServerName());
		REQUIRE_FALSE(servers[0].hasMaxBody());
		REQUIRE_FALSE(servers[0].hasRoutes());
		REQUIRE_FALSE(servers[1].hasServerName());
		REQUIRE_FALSE(servers[1].hasMaxBody());
		REQUIRE_FALSE(servers[1].hasRoutes());
		destructConfig();
	}
	configTest++;

	// parser_2
	WHEN(getFileDest(fileDest, configTest)) {
		int ret = initConfigTest(fileDest, configTest);
		REQUIRE(ret == 0);
		REQUIRE(servers.size() == 2);
		REQUIRE_FALSE(servers[0].hasServerName());
		REQUIRE_FALSE(servers[0].hasMaxBody());
		REQUIRE_FALSE(servers[0].hasRoutes());
		REQUIRE_FALSE(servers[1].hasServerName());
		REQUIRE_FALSE(servers[1].hasMaxBody());
		REQUIRE_FALSE(servers[1].hasRoutes());
		destructConfig();
	}
	configTest++;

}

