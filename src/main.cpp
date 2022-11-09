#include "Multiplexer.hpp"

int main() {
  std::vector<Server> servers;      // vector of servers
  Multiplexer         multiplexer;  // the multiplexer object that is responsible for handling all the servers

  // int i = parseConfigFile( &servers, argv[1]); // fills the servers vector with the servers from the config file
  // ^ this shouldn't be throwing exceptions, since the config file is checked for errors before the program is started

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
