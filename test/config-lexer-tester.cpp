#define CONFIG_CATCH_MAIN
#include "catch.hpp"
#include <string>
#include <iostream>
#include <fstream>
#include <Lexer.hpp>

int assertTokenType(std::vector<Token>::iterator *it, ETokenType expected)
{
}

void openFile(std::ifstream &file, std::string path)
{
	file.open(path);
	if (!file.is_open())
	{
		WARN("Error opening file");
	}
}

SCENARIO("Incorrect lexing input")
{
	GIVEN("An unopened file")
	{
		std::ifstream unopenedFile;

		THEN("Lexer should throw error")
		{
			// simulate error
			REQUIRE(unopenedFile.is_open() == true);
		}
	}
}

SCENARIO("Empty config file")
{
	std::ifstream emptyFile;
	openFile(emptyFile, "testconfigs/empty");
	std::vector<Token> tokens;

	// REQUIRE(emptyFile.is_open() == true);

	WHEN("Empty config file is passed")
	{
		tokens = Lexer::tokenizeFile(emptyFile);
		THEN("Should return empty list of tokens")
		{
			// REQUIRE(true);
		}
	}
}

// SCENARIO("Basic config files") {
// 	std::string	testConfPath("testconfigs/basic_");
// 	std::vector<Token>	tokens;
// 	std::vector<Token>::iterator it = tokens.begin()

// 	SECTION("basic_0") {
// 		testConfPath.append("0");
// 		tokens = Lexer::tokenizeFile(testConfPath);

// 	}
// }
