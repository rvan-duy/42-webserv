#include "Socket.hpp"

#define SERVER_PORT 8080

int main() {
  {
    Socket *listen_socket = new Socket(AF_INET, SOCK_STREAM, 0, SERVER_PORT);
    listen_socket->prepare_connections(10);
    listen_socket->accept_connection();
    delete listen_socket;
  }

  return 0;
}
