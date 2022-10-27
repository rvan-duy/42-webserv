#include "Multiplexer.hpp"

#define SERVER_PORT 8080

int main() {
  {
    try {
      Multiplexer multiplexer;
      Socket      socket1(AF_INET, SOCK_STREAM, 0, SERVER_PORT);

      multiplexer.add_socket(&socket1, POLLIN);

      multiplexer.wait_for_events();

      multiplexer.remove_socket(&socket1);

    } catch (std::exception &e) {
      std::cerr << e.what() << std::endl;
    }
  }

  return 0;
}
