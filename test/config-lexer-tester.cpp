#define CONFIG_CATCH_MAIN
#include "catch.hpp"
#include <string>
#include <iostream>
#include "../include/Lexer.hpp"

int		assertTokenType(std::vector<Token>::iterator *it, ETokenType expected) {

}

std::ofstream	openFile(std::string path) {
	std::ofstream	file;

	file.open(path);
	if (!file.is_open) {
		INFO("Error opening file");
	}
	return file;
}

SCENARIO("Incorrect lexing input") {
	GIVEN("An unopened file") {
		std::ifstream unopenedFile;

		THEN("Lexer should throw error") {
			// simulate error
		}
	}
}

SCENARIO("Empty config file") {
	std::ifstream emptyFile = openFile("testconfigs/empty");
	std::vector<Token>	tokens;

	REQUIRE(emptyFile.is_open());

	WHEN("Empty config file is passed") {
		tokens = Lexer::tokenizeFile(emptyFile);
		THEN("Should return empty list of tokens") {
			REQUIRE(tokens.size == 0);
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
