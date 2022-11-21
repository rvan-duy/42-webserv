#define CONFIG_CATCH_MAIN
#include "catch.hpp"
#include <CGI.hpp>
#include <iostream>
#include <cstdlib>

SCENARIO("Simple python file testing") {
	char *const env[] = {
		"USER=rcappend"
		"SECURITYSESSIONID=186a7"
		"XPC_FLAGS=0x0"
		"__CF_USER_TEXT_ENCODING=0x12EBB:0x0:0x0"
		"ORIGINAL_XDG_CURRENT_DESKTOP=undefined"
		"TERM_PROGRAM=vscode"
		"TERM_PROGRAM_VERSION=1.73.0"
		"LANG=en_US.UTF-8"
	};
	CGI testCgi("/test/cgi/", env);

	std::string dest;
	REQUIRE (testCgi.executeFile(&dest, "test/cgi/helloworld.py", "yes") == 0);
	std::cout << dest << std::endl;
}
