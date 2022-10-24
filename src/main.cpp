#include <iostream>
#include <Logger.hpp>
#include <ConfigParser.hpp>
#include "Socket.hpp"

#define SERVER_PORT 8080

void  parseConfigFile(std::string const& filePath) {
  ConfigParser parser(filePath);
}

int main(int argc, char **argv) {
  Logger& logger = Logger::getInstance();
  if (argc != 2) {
    logger.error("Usage: ./webserver [path_to_configfile]");
    return EXIT_FAILURE;
  }
  parseConfigFile(argv[1]);

  try {
      Socket socket(AF_INET, SOCK_STREAM, 0, SERVER_PORT);
      socket.prepare(10);
      socket.wait_for_connections();
    } catch (std::exception &e) {
      std::cerr << e.what() << std::endl;
    }
  // Great success
  return EXIT_SUCCESS;
}