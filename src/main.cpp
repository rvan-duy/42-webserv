#include "Socket.hpp"

#define SERVER_PORT 8080

int main() {
  {
    try {
      Socket socket(AF_INET, SOCK_STREAM, 0, SERVER_PORT);
      socket.prepare(10);
      socket.wait_for_connections();
    } catch (std::exception &e) {
      std::cerr << e.what() << std::endl;
    }
  }

  return 0;
}
