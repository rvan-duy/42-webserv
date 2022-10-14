#ifndef SOCKET_HPP
#define SOCKET_HPP

#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>

#include <cstdlib>
#include <iostream>

/****************************************************************************************************/
/* The Socket Class                                                                                 */
/* - A socket is a mechanism that provides programs access to the network, it allows messages to be */
/*   sent and received between programs.                                                            */
/* - A socket is a combination of an IP address and a port number.                                  */
/* - A socket is a file descriptor, and is used to read and write data to the network.              */
/****************************************************************************************************/
/* A few steps are involved with sockets                                                            */
/* 1. Create a socket                                                                               */
/* 2. Bind the socket to an address and port number (also known as identifying the socket,          */
/*      giving it a name..)                                                                         */
/* 3. Wait for an incoming connection                                                              */
/* 4. Send and receive messages                                                                     */
/* 5. Close socket                                                                                  */
/****************************************************************************************************/

// IMPORTANT: all these things can throw exceptions, so we need to handle them in the main function

class Socket {
 public:
  Socket(const int domain, const int type, const int protocol, const int port);
  ~Socket();

  // Methods
  // TODO: come up with better names for these methods
  void prepare_connections(const int backlog);
  void accept_connection();

 private:
  int                _fd;        // file descriptor for socket
  const int          _port;      // port number of socket
  struct sockaddr_in _servaddr;  // socket address structure
                                 /*__uint8_t       sin_len;
                                   sa_family_t     sin_family;
                                   in_port_t       sin_port;
                                   struct  in_addr sin_addr;
                                   char            sin_zero[8]; */
};

#endif  // SOCKET_HPP
