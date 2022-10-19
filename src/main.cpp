#include <iostream>
#include <Logger.hpp>

#ifndef LOGLEVEL
# define LOGLEVEL 0
#endif

int main() {
  std::cout << "Hello Webserver!" << std::endl;
  Logger& logger = Logger::GetInstance();
  logger.log("Hi");
  // Great success
  return EXIT_SUCCESS;
}
