#include "catch.hpp"

SCENARIO("Another example test") {
  GIVEN("An empty vector") {
    std::vector<std::string> v;

    // Validate assumption of the GIVEN clause
    THEN("The size and capacity start at 0") {
      REQUIRE(v.size() == 0);
      REQUIRE(v.capacity() == 0);
    }

    // Validate one use case for the GIVEN object
    WHEN("push_back() is called") {
      v.push_back("hullo");

      THEN("The size changes") {
        REQUIRE(v.size() == 1);
        REQUIRE(v.capacity() >= 1);
      }
    }
  }
}
