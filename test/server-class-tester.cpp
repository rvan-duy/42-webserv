#define CONFIG_CATCH_MAIN
#include "catch.hpp"
#include <string>
#include <Server.hpp>

SCENARIO("Constructing a new server class")
{
	GIVEN("A new server class")
	{
		Server testServer;
		THEN("All has X functions should return false")
		{
			REQUIRE_FALSE(testServer.hasServerName());
			REQUIRE_FALSE(testServer.hasMaxBody());
		}

		THEN("Server name can be set correctly")
		{
			std::vector<std::string> serverName;
			serverName.push_back("test");
			testServer.setServerName(serverName);
			REQUIRE(testServer.hasServerName());
			REQUIRE(testServer.getServerName() == serverName);
		}

		THEN("Host can be set correctly")
		{
			testServer.setHost(300, "bla");
			PageData host = testServer.getHost();
			REQUIRE(host.statusCode == 300);
			REQUIRE(host.filePath == "bla");
		}

		THEN("Error page can be set correctly")
		{
			testServer.setErrorPage(300, "bla");
			PageData host = testServer.getErrorPage();
			REQUIRE(host.statusCode == 300);
			REQUIRE(host.filePath == "bla");
		}

		THEN("Port can be set correctly")
		{
			unsigned int port = 10;
			testServer.setPort(port);
			REQUIRE(testServer.getPort() == port);
		}

		THEN("Max body can be set correctly")
		{
			unsigned int maxBody = 10;
			testServer.setMaxBody(maxBody);
			REQUIRE(testServer.hasMaxBody());
			REQUIRE(testServer.getMaxBody() == maxBody);
		}
	}
}

SCENARIO("Constructing a new server class with incorrect inputs")
{
	GIVEN("A new server class")
	{
		Server testServer;

		THEN("Port can't be set incorrectly")
		{
			// Higher than max port
			REQUIRE(testServer.setPort(MAX_PORT + 1) == 1);
			REQUIRE(testServer.setPort(0) == 1);
			REQUIRE(testServer.setPort(-100) == 1);
		}

		THEN("Max body can't be set incorrectly")
		{
			// Lower than 0
			REQUIRE(testServer.setMaxBody(0) == 1);
			REQUIRE_FALSE(testServer.hasMaxBody());
			REQUIRE(testServer.setMaxBody(UINT_MAX + 1) == 1);
			REQUIRE_FALSE(testServer.hasMaxBody());
		}
	}
}
