#ifndef SOCKET_HPP
#define SOCKET_HPP

#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>
#include <sys/poll.h>

#include <cstdlib>
#include <fstream>
#include <iostream>
#include <cstring>
#include <sstream>

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
/* 3. Wait for an incoming connection                                                               */
/* 4. Send and receive messages                                                                     */
/* 5. Close socket                                                                                  */
/****************************************************************************************************/

// IMPORTANT: all these things can throw exceptions, so we need to handle them in the main function

#include "HttpRequest.hpp"
#include "HttpResponse.hpp"

class Socket {
 public:
  Socket(const int domain, const int type, const int protocol, const int port);
  ~Socket();

  // Methods
  void prepare(const int backlog = 10) const;
  void wait_for_connections();

  // Getters
  int get_fd() const;

 private: 
  int                _fd;        // file descriptor for socket
  const int          _port;      // port number of socket
  struct sockaddr_in _servaddr;  // socket address structure
                                 /*__uint8_t       sin_len;
                                   sa_family_t     sin_family;
                                   in_port_t       sin_port;
                                   struct  in_addr sin_addr;
                                   char            sin_zero[8]; */

  std::string get_response_to_str(const HttpRequest &request) const;
};

#endif  // SOCKET_HPP
