#include <iostream>
#include <Logger.hpp>

int main(int argc, char **argv) {
  if (argc != 2) {
    Logger::error("Usage: ./webserver [path_to_configfile]");
    return EXIT_FAILURE;
  }
  // Great success
  return EXIT_SUCCESS;
}
