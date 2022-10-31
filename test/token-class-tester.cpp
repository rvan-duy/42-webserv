#define CONFIG_CATCH_MAIN
#include "catch.hpp"
#include <string>
#include <Token.hpp>
#include <iostream>

SCENARIO("New token class is created") {
	GIVEN("A token constructed with a string") {
		Token	token("test");
		THEN("Type must be correct") {
			REQUIRE(token.getType() == Token::WORD);
			REQUIRE(token.getWord() == "test");
		}

		THEN("Is equal functions should work") {
			REQUIRE(token.isWordEqualTo("test") == true);
			REQUIRE(token.isType(Token::WORD) == true);
		}
	}

	GIVEN("A token constructed with an enum") {
		Token	token(Token::CLOSE_CURL);
		THEN("Type must be correct") {
			REQUIRE(token.getType() == Token::CLOSE_CURL);
			REQUIRE(token.getWord() == "");
		}

		THEN("Is equal functions should work") {
			REQUIRE(token.isWordEqualTo("test") == false);
			REQUIRE(token.isType(Token::CLOSE_CURL) == true);
		}
	}
}