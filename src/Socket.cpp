#include "Socket.hpp"

Socket::Socket(const int domain, const int type, const int protocol, const int port) {
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
  /* Bind our socket address to the                 */
  /* listening socket, and call listen()            */
  /**************************************************/

  if (bind(_fd, (struct sockaddr *)&_servaddr, sizeof(_servaddr)) < 0) {
    throw std::runtime_error("Socket bind failed: " + std::string(strerror(errno)));
  }

  if (listen(_fd, 10) < 0) {
    throw std::runtime_error("Socket listen failed: " + std::string(strerror(errno)));
  }
}

Socket::~Socket() {
  close(_fd);
}
