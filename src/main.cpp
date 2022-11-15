#include "Multiplexer.hpp"
#include <Parser.hpp>
#include <Lexer.hpp>

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
	if (parser.parseTokens(&servers)) {
		logger.error("Error parsing tokens from configfile");
		return 1;
	}
	logger.log("Tokens successfully parsed");
	cFile.close();
	return 0;
}

int main(int argc, char **argv) {
  if (argc != 2) {
    std::cout << "Usage: " << argv[0] << " <config_file>" << std::endl;
    // return 1; TMP
  }

  // parseConfigFile(argv[1]);

  std::vector<Server> servers;      // vector of servers
  Multiplexer         multiplexer;  // the multiplexer object that is responsible for handling all the servers

  /* TEMPORARY CODE */
  Server my_server = Server();  // create a server object
  std::vector<std::string> serverName;
  serverName.push_back("localhost");
  my_server.setServerName(serverName);
  my_server.setPort(8080);
  my_server.setHost(200, "index.html");
  my_server.setErrorPage(404, "404");
  servers.push_back(my_server);
  /* END OF TEMPORARY CODE */

  try {
    // Iterate through all servers and add them to the multiplexer
    for (std::vector<Server>::iterator it = servers.begin(); it != servers.end(); ++it) {
      it->prepare(); // socket() + bind() + listen()
      multiplexer.addServer(*it, POLLIN);
    }
    multiplexer.waitForEvents(); // Start the multiplexer loop, does all the polling and handling of events etc.
  } catch (std::exception &e) {
    std::cerr << e.what() << std::endl;
  }
  return 0;
}
