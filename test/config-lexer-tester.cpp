#define CONFIG_CATCH_MAIN
#include "catch.hpp"
#include <string>
#include <iostream>
#include <fstream>
#include <Lexer.hpp>

/**************************************************/
/* Testing template                         		*/
/**************************************************/
/*
SCENARIO("Name here")
{
	std::string	fileDest("Filepath base url here");
	int i = 0;

	WHEN(getFileDest(fileDest, i)) {
		initTest(fileDest, i);

		// Asserts here

		destructTest();
	}
}
*/

/**
 * Globals used for testing
*/
std::vector<Token> tokens;
std::vector<Token>::iterator it;
std::ifstream file;

/**
 * Token assertions
*/
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

/**
 * Test init & destruct functions
*/
int openFile(std::string path)
{
	file.close();
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
std::string	getFileDest(std::string &baseUrl, int currentTest) {
	std::string fullDest(baseUrl);
	return fullDest.append(std::to_string(currentTest));
}

void	initTest(std::string baseUrl, int i) {
	std::string fullUrl = getFileDest(baseUrl, i);
	if (openFile(fullUrl)) {
		WARN("Error opening file");
		return ;
	}
	tokens = Lexer::tokenizeFile(file);
	it = tokens.begin();
}

void	destructTest() {
	REQUIRE(it == tokens.end());
	tokens.clear();
	file.close();
}

/**************************************************/
/* TEST					                           */
/**************************************************/
SCENARIO("Incorrect lexing input")
{
	GIVEN("An unopened file")
	{
		THEN("Lexer should return empty tokenlist")
		{
			file.close();
			// simulate error
			REQUIRE(file.is_open() == false);
			tokens = Lexer::tokenizeFile(file);
			REQUIRE(tokens.size() == 0);
			destructTest();
		}
	}
}

SCENARIO("(Semi-) empty config files")
{
	std::string	fileDest("testconfigs/empty_");
	int i = 0;

	// empty_0
	WHEN(getFileDest(fileDest, i)) {
		initTest(fileDest, i);

		REQUIRE(tokens.size() == 0);

		destructTest();
	}
	
	i += 1;
	// empty_1
	WHEN(getFileDest(fileDest, i)) {
		initTest(fileDest, i);

		REQUIRE(tokens.size() == 0);

		destructTest();
	}

	i += 1;
	// empty_2
	WHEN(getFileDest(fileDest, i)) {
		initTest(fileDest, i);

		REQUIRE(tokens.size() == 2);
		assertWord("test");
		assertToken(OPEN_CURL);

		destructTest();
	}
}

SCENARIO("Simple key value pairs")
{
	std::string	fileDest("testconfigs/key_value_");
	int i = 0;

	// key_value_0
	WHEN(getFileDest(fileDest, i)) {
		initTest(fileDest, i);

		REQUIRE(tokens.size() == 3);
		assertWord("test");
		assertWord("blabla");
		assertToken(SEMICOLON);

		destructTest();
	}
	
	i += 1;
	// key_value_1
	WHEN(getFileDest(fileDest, i)) {
		initTest(fileDest, i);

		REQUIRE(tokens.size() == 6);
		assertWord("test");
		assertWord("blabla");
		assertToken(SEMICOLON);
		assertWord("test");
		assertWord("blabla");
		assertToken(SEMICOLON);

		destructTest();
	}

	i += 1;
	// key_value_2
	WHEN(getFileDest(fileDest, i)) {
		initTest(fileDest, i);

		REQUIRE(tokens.size() == 4);
		assertWord("bla");
		assertWord("bla");
		assertToken(OPEN_CURL);
		assertToken(CLOSE_CURL);

		destructTest();
	}
}

SCENARIO("Lots of icons")
{
	std::string	fileDest("testconfigs/icons_");
	int i = 0;

	WHEN(getFileDest(fileDest, i)) {
		initTest(fileDest, i);

		REQUIRE(tokens.size() == 5);
		assertToken(OPEN_CURL);
		assertToken(CLOSE_CURL);
		assertToken(SEMICOLON);
		assertWord("word");
		assertToken(SEMICOLON);
		destructTest();
	}

	i++;
	WHEN(getFileDest(fileDest, i)) {
		initTest(fileDest, i);

		REQUIRE(tokens.size() == 6);
		assertToken(OPEN_CURL);
		assertToken(SEMICOLON);
		assertToken(CLOSE_CURL);
		assertToken(SEMICOLON);
		assertToken(OPEN_CURL);
		assertToken(CLOSE_CURL);
		
		destructTest();
	}
}