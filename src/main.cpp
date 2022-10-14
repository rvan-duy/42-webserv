#include "Socket.hpp"

#define SERVER_PORT 8080

int main() {
  {
    Socket *listen_socket = new Socket(AF_INET, SOCK_STREAM, 0, SERVER_PORT);
    delete listen_socket;
  }

  return 0;
}
