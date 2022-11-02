#define CONFIG_CATCH_MAIN
#include "catch.hpp"
#include <string>
#include <iostream>
#include <fstream>
#include <Lexer.hpp>
#include <stdio.h>

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

/**
 * Token assertions
*/
void	assertToken(Token::ETokenType expected)
{
	REQUIRE(it->getType() == expected);
	REQUIRE(it->getWord() == "");
	it++;
}

void	assertTokenWord(std::string expected)
{
	REQUIRE(it->getType() == Token::WORD);
	REQUIRE(it->getWord() == expected);
	it++;
}

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

void	initTest(std::string baseUrl, int i) {
	// Open file
	std::ifstream file;
	std::string fullUrl = getFileDest(baseUrl, i);
	if (openFile(file, fullUrl)) {
		return ;
	}
	// Lex file
	tokens = Lexer::tokenizeFile(file);
	file.close();
	it = tokens.begin();
}

void	destructTest() {
	REQUIRE(it == tokens.end());
	tokens.clear();
}

/**************************************************/
/* TESTS					                      */
/**************************************************/
SCENARIO("Incorrect lexing input")
{
	GIVEN("An unopened file")
	{
		THEN("Lexer should return empty tokenlist")
		{
			// simulate error
			std::ifstream file;
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
	i++;
	
	// empty_1
	WHEN(getFileDest(fileDest, i)) {
		initTest(fileDest, i);

		REQUIRE(tokens.size() == 0);

		destructTest();
	}
	i++;

	// empty_2
	WHEN(getFileDest(fileDest, i)) {
		initTest(fileDest, i);

		REQUIRE(tokens.size() == 2);
		assertTokenWord("test");
		assertToken(Token::OPEN_CURL);

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
		assertTokenWord("test");
		assertTokenWord("blabla");
		assertToken(Token::SEMICOLON);

		destructTest();
	}
	i++;
	
	// key_value_1
	WHEN(getFileDest(fileDest, i)) {
		initTest(fileDest, i);

		REQUIRE(tokens.size() == 6);
		assertTokenWord("test");
		assertTokenWord("blabla");
		assertToken(Token::SEMICOLON);
		assertTokenWord("test");
		assertTokenWord("blabla");
		assertToken(Token::SEMICOLON);

		destructTest();
	}
	i++;

	// key_value_2
	WHEN(getFileDest(fileDest, i)) {
		initTest(fileDest, i);

		REQUIRE(tokens.size() == 4);
		assertTokenWord("bla");
		assertTokenWord("bla");
		assertToken(Token::OPEN_CURL);
		assertToken(Token::CLOSE_CURL);

		destructTest();
	}
}

SCENARIO("Lots of icons")
{
	std::string	fileDest("testconfigs/icons_");
	int i = 0;

	// icons_0
	WHEN(getFileDest(fileDest, i)) {
		initTest(fileDest, i);

		REQUIRE(tokens.size() == 5);
		assertToken(Token::OPEN_CURL);
		assertToken(Token::CLOSE_CURL);
		assertToken(Token::SEMICOLON);
		assertTokenWord("word");
		assertToken(Token::SEMICOLON);
		destructTest();
	}
	i++;

	// icons_1
	WHEN(getFileDest(fileDest, i)) {
		initTest(fileDest, i);

		REQUIRE(tokens.size() == 6);
		assertToken(Token::OPEN_CURL);
		assertToken(Token::SEMICOLON);
		assertToken(Token::CLOSE_CURL);
		assertToken(Token::SEMICOLON);
		assertToken(Token::OPEN_CURL);
		assertToken(Token::CLOSE_CURL);
		
		destructTest();
	}
	i++;
}
