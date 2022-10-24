#include <iostream>
#include <webserver.hpp>
#include <Logger.hpp>
#include "Socket.hpp"

#define SERVER_PORT 8080

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
