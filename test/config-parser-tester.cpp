#define CONFIG_CATCH_MAIN
#include "catch.hpp"
#include <string>

SCENARIO("Initializing config parser") {
	GIVEN("An incorrect filepath") {
		std::string	filePath("blabla");

		WHEN("Constructor is called with incorrect filepath") {
			
		}
	}
}