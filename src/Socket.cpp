#include "Socket.hpp"

/*
 * Constructor
 * @param domain communication domain
 * @param type communication semantics
 * @param protocol protocol to be used with the socket
 * @param port port number of socket
 */
Socket::Socket(const int domain, const int type, const int protocol, const int port) : _port(port) {
  /**************************************************/
  /* Create an socket to receive incoming           */
  /* connections on                                 */
  /**************************************************/

  _fd = socket(domain, type, protocol);
  if (_fd == -1) {
    throw std::runtime_error("Socket creation failed: " + std::string(strerror(errno)));
  }

  /**************************************************/
  /* Set all bytes in socket address structure to   */
  /* zero, and fill in the relevant data members    */
  /**************************************************/

  memset(&_servaddr, 0, sizeof(_servaddr));
  _servaddr.sin_family      = domain;
  _servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
  _servaddr.sin_port        = htons(port);

  /**************************************************/
  /* Bind the address and port number to the socket */
  /**************************************************/

  if (bind(_fd, (struct sockaddr *)&_servaddr, sizeof(_servaddr)) == -1) {
    throw std::runtime_error("Socket bind failed: " + std::string(strerror(errno)));
  }
}

/*
 * listen
 * @param backlog number of connections allowed on the incoming queue
 */
void Socket::listen(const int backlog) {
  /**************************************************/
  /* Listen for incoming connections.               */
  /**************************************************/

  if (::listen(_fd, backlog) == -1) {
    throw std::runtime_error("Socket listen failed: " + std::string(strerror(errno)));
  }
}

/*
 * Destructor
 */
Socket::~Socket() {
  /**************************************************/
  /* Close the socket                               */
  /**************************************************/

  if (close(_fd) == -1) {
    throw std::runtime_error("Socket close failed: " + std::string(strerror(errno)));
  }
}
