#define CONFIG_CATCH_MAIN
#include "catch.hpp"
#include <string>
#include <iostream>
#include <fstream>
#include <Lexer.hpp>

std::vector<Token> tokens;
std::vector<Token>::iterator it;

void	resetVars() {
	tokens.clear();
	it = tokens.begin();
}

void	assertToken(ETokenType expected)
{
	REQUIRE(it->getType() == expected);
	REQUIRE(it->getWord() == "");
	it++;
}

void	assertWord(std::string expected)
{
	REQUIRE(it->getType() == WORD);
	REQUIRE(it->getWord().compare(expected) == 0);
	it++;
}

// Used to generate file urls with testcase added ot the back
// Example: testconfigs/key_value_[testnumber]
std::string	getFileDest(std::string &baseUrl, int currentTest) {
	std::string fullDest(baseUrl);
	return fullDest.append(std::to_string(currentTest));
}

int openFile(std::ifstream &file, std::string path)
{
	file.open(path);
	if (!file.is_open())
	{
		WARN("Error opening file");
		return 1;
	}
	return 0;
}

SCENARIO("Incorrect lexing input")
{
	GIVEN("An unopened file")
	{
		std::ifstream unopenedFile;

		THEN("Lexer should return empty tokenlist")
		{
			// simulate error
			REQUIRE(unopenedFile.is_open() == false);
			tokens = Lexer::tokenizeFile(unopenedFile);
			REQUIRE(tokens.size() == 0);
			resetVars();
		}
	}
}

SCENARIO("Empty config file")
{
	std::ifstream emptyFile;
	openFile(emptyFile, "testconfigs/empty");

	REQUIRE(emptyFile.is_open() == true);
	WHEN("Empty config file is passed")
	{
		tokens = Lexer::tokenizeFile(emptyFile);
		THEN("Should return empty list of tokens")
		{
			REQUIRE(tokens.size() == 0);
		}
	}
}

SCENARIO("Simple key value pairs")
{
	std::string	fileDest("testconfigs/key_value_");
	std::ifstream cFile;
	int i = 0;

	// key_value_0
	WHEN(getFileDest(fileDest, i)) {
		if (openFile(cFile, getFileDest(fileDest, i))) {
			WARN("Error opening file");
			return ;
		}
		std::vector<Token> tokens = Lexer::tokenizeFile(cFile);
		REQUIRE(tokens.size() == 3);
	}
	i++;

	// key_value_1
	WHEN(getFileDest(fileDest, i)) {
		if (openFile(cFile, getFileDest(fileDest, i))) {
			WARN("Error opening file");
			return ;
		}
		std::vector<Token> tokens = Lexer::tokenizeFile(cFile);
		REQUIRE(tokens.size() == 3);
	}
	i++;
}