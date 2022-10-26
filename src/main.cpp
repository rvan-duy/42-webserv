#include "Socket.hpp"

#define SERVER_PORT 8080

int main() {
  {
    try {
      struct pollfd fds[1];
      Socket        socket(AF_INET, SOCK_STREAM, 0, SERVER_PORT);

      fds->fd = socket.get_fd();
      fds->events = POLLIN;

      socket.prepare();
      socket.wait_for_connections();
    } catch (std::exception &e) {
      std::cerr << e.what() << std::endl;
    }
  }

  return 0;
}
