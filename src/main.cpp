#include <iostream>
#include <Logger.hpp>
#include <Lexer.hpp>
#include <Parser.hpp>
#include "Socket.hpp"

#define SERVER_PORT 8080

int	parseConfigFile(std::string const& filePath) {
	Logger& logger = Logger::getInstance();
	std::ifstream cFile;

	cFile.open(filePath);
	if (!cFile.is_open()) {
		logger.error("Error opening file: " + std::string(strerror(errno)));
		return 1;
	}
	logger.log("Opened config file");

  	std::vector<Token> tokens = Lexer::tokenizeFile(cFile);
	std::vector<Server> servers;
	Parser	parser(tokens);
	if (parser.parseTokens()) {
		logger.error("Error parsing tokens from configfile");
		return 1;
	}
	logger.log("Tokens successfully parsed");
	cFile.close();
	return 0;
}

int main(int argc, char **argv) {
  Logger& logger = Logger::getInstance();
  if (argc != 2) {
    logger.error("Usage: ./webserver [path_to_configfile]");
    return EXIT_FAILURE;
  }
	parseConfigFile(argv[1]);	
  // Great success
  return EXIT_SUCCESS;
}
