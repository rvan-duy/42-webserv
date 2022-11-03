#include "Multiplexer.hpp"

#define SERVER_PORT 8080

int main() {
  {
    try {
      Multiplexer multiplexer;
      Socket      socket(AF_INET6, SOCK_STREAM, SERVER_PORT);

      socket.prepare();
      multiplexer.add_server(socket.get_fd(), POLLIN);
      multiplexer.wait_for_events();
    } catch (std::exception &e) {
      std::cerr << e.what() << std::endl;
    }
  }

  return 0;
}
