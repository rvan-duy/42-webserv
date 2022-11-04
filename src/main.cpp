#include "Multiplexer.hpp"

#define SERVER_PORT 8080

int main() {
  {
    try {
      Multiplexer multiplexer;
      Socket      socket(AF_INET6, SOCK_STREAM, SERVER_PORT);

      socket.prepare();
      multiplexer.addServer(socket.getFd(), POLLIN);
      multiplexer.waitForEvents();
    } catch (std::exception &e) {
      std::cerr << e.what() << std::endl;
    }
  }

  return 0;
}
