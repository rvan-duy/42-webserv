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
	// good_0
	WHEN(getFileDest(fileDest, configTest)) {
		int ret = initConfigTest(fileDest, configTest);
		REQUIRE(ret == 0);
		REQUIRE(servers.size() == 2);

		THEN("Port should be set for first server") {
			REQUIRE(servers[0].hasPort());
			REQUIRE(servers[0].getPort() == 80);
		}

		THEN("Server name should be set for first server") {
			REQUIRE(servers[0].hasServerName());
			std::vector<std::string> serverName = servers[0].getServerName();
			REQUIRE(serverName.size() == 2);
			REQUIRE(serverName.at(0) == "localhost");
			REQUIRE(serverName.at(1) == "www.localhost");
		}
		
		THEN("Max body shouldnt be set for first server") {
			REQUIRE_FALSE(servers[0].hasMaxBody());
		}

		THEN("Error page should be set for first server") {
			PageData	data = servers[0].getErrorPage();
			REQUIRE(data.statusCode == 404);
			REQUIRE(data.filePath == "/error.html");
		}

		THEN("Host should be set for first server") {
			PageData	data = servers[0].getHost();
			REQUIRE(data.statusCode == 302);
			REQUIRE(data.filePath == "http://localhost:8200$uri");
		}

		THEN("First server should have four routes") {
			std::vector<Route> routes = servers[0].getRoutes();
			REQUIRE(routes.size() == 4);
		}
		
		THEN("Port should be set for second server") {
			REQUIRE(servers[1].hasPort());
			REQUIRE(servers[1].getPort() == 100);
		}

		THEN("max body should be set for second server") {
			REQUIRE(servers[1].hasMaxBody());
			REQUIRE(servers[1].getPort() == 100);
		}

		destructConfig();
	}
	configTest++;

}

