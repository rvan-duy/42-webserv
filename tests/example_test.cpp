#include "catch2/catch.hpp"

SCENARIO("Example test") {
  GIVEN("An integer") {
    int i = 1;
    WHEN("The integer is incremented") {
      i++;
      THEN("The integer is equal to 2") {
        REQUIRE(i == 2);
      }
    }
  }
}
