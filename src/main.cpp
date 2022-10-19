#include <iostream>
#include <Logger.hpp>
#include <ConfigParser.hpp>

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
  // Great success
  return EXIT_SUCCESS;
}
