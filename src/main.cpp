#include <iostream>
#include <Logger.hpp>

int main(int argc, char **argv) {
  Logger& logger = Logger::getInstance();
  if (argc != 2) {
    logger.error("Usage: ./webserver [path_to_configfile]");
    return EXIT_FAILURE;
  }
  // Great success
  return EXIT_SUCCESS;
}
