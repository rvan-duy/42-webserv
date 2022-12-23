#define CONFIG_CATCH_MAIN
#include <CGI.hpp>
#include <cstdlib>
#include <iostream>

#include "catch.hpp"

SCENARIO("Simple python file testing") {
  char *const env[] = {
      "USER=rcappend"
      "SECURITYSESSIONID=186a7"
      "XPC_FLAGS=0x0"
      "__CF_USER_TEXT_ENCODING=0x12EBB:0x0:0x0"
      "ORIGINAL_XDG_CURRENT_DESKTOP=undefined"
      "TERM_PROGRAM=vscode"
      "TERM_PROGRAM_VERSION=1.73.0"
      "LANG=en_US.UTF-8"};
  CGI testCgi(env);

  std::string dest;
  std::vector<std::string> headers;
  REQUIRE(testCgi.executeFile(&dest, &headers, "test/cgi/helloworld.py",
                              "yes") == HTTPStatusCode::OK);
  std::cout << dest << std::endl;
}
