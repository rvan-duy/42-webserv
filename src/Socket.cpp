#include "Socket.hpp"

#include "Logger.hpp"

/*
 * Constructor for Socket class
 * @param domain communication domain (AF_INET, AF_INET6, AF_LOCAL, AF_ROUTE, AF_KEY)
 * @param type communication semantics (SOCK_STREAM, SOCK_DGRAM, SOCK_SEQPACKET, SOCK_RAW)
 * @param protocol protocol to be used with the socket (0 for default)
 * @param port port number of socket (0 for random)
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
  /* Set options on socket to reuse address         */
  /**************************************************/

  int opt = 1;
  setsockopt(_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

  /**************************************************/
  /* Set all bytes in socket address structure to   */
  /* zero, and fill in the relevant data members    */
  /**************************************************/

  memset(&_servaddr, 0, sizeof(_servaddr));
  _servaddr.sin_family      = domain;
  _servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
  _servaddr.sin_port        = htons(_port);

  /**************************************************/
  /* Bind the address and port number to the socket */
  /**************************************************/

  if (bind(_fd, (struct sockaddr *)&_servaddr, sizeof(_servaddr)) == -1) {
    throw std::runtime_error("Socket bind failed: " + std::string(strerror(errno)));
  }
}

/*
 * Prepare the socket for incoming connections
 * @param backlog number of connections allowed on the incoming queue
 */
void Socket::prepare(const int backlog) const {
  if (listen(_fd, backlog) == -1) {
    throw std::runtime_error("Socket listen failed: " + std::string(strerror(errno)));
  }
}

// Poll is used to check if blocking functions can be called without actually blocking the thread

/*
 * Wait for incoming connections
 */
void Socket::wait_for_connections() {
  Logger         &logger   = Logger::getInstance();
  const socklen_t sock_len = sizeof(_servaddr);  // size of socket address structure
  int             new_fd;                        // file descriptor for new socket
  char            buffer[100000] = {0};          // buffer to read incoming data into

  /**************************************************/
  /* Start accepting incoming connections           */
  /**************************************************/

  while (1) {
    {  // This block is just for the logger
      std::ostringstream ss;
      ss << "Waiting for connections on port " << _port;
      logger.log(ss.str());
    }

    /**************************************************/
    /* Accept an incoming connection                  */
    /* - Blocks until a connection is present         */
    /* - Because the accept() function expects a      */
    /* sockaddr structure, it needs to be cast to the */
    /* correct type, the sockaddr_in structure is a   */
    /* "subclass" of sockaddr, so we can cast it to a */
    /* sockaddr structure                             */
    /**************************************************/

    if ((new_fd = accept(_fd, (struct sockaddr *)&_servaddr, (socklen_t *)&sock_len)) == -1) {
      logger.error("Socket accept failed: " + std::string(strerror(errno)));
      throw std::runtime_error("Socket accept failed: " + std::string(strerror(errno)));
    }

    /**************************************************/
    /* Read data from the incoming connection         */
    /**************************************************/

    logger.log("Reading data from socket");

    bzero(buffer, sizeof(buffer));
    ssize_t bytes_read = read(new_fd, buffer, sizeof(buffer));
    if (bytes_read == -1) {
      logger.error("Socket read failed: " + std::string(strerror(errno)));
      throw std::runtime_error("Socket read failed: " + std::string(strerror(errno)));
    }

    if (bytes_read == 0) {
      logger.log("Socket read 0 bytes, ignoring");
      continue;
    }

    {  // This block is just for the logger
      std::ostringstream ss;
      ss << "Successfully read " << bytes_read << " bytes from socket";
      logger.log(ss.str());
    }

    /**************************************************/
    /* Log the received message                       */
    /**************************************************/

    logger.log("Received request\n---------------------------\n" + std::string(buffer) +
               "\n---------------------------\n");

    /**************************************************/
    /* Send response to the client                    */
    /**************************************************/

    {
      HttpRequest  request;
      HttpResponse response;

      request.parse(buffer);
      response.create_response(request, "root",
                               "index.html");  // TODO: make root configurable, not hardcoded, same for index

      std::string response_str = response.to_str();

      if (write(new_fd, response_str.c_str(), response_str.length()) == -1) {
        logger.error("Socket write failed: " + std::string(strerror(errno)));
        throw std::runtime_error("Socket write failed: " + std::string(strerror(errno)));
      }
    }

    /**************************************************/
    /* Close the connection                           */
    /**************************************************/

    if (close(new_fd) == -1) {
      logger.error("Socket close failed: " + std::string(strerror(errno)));
      throw std::runtime_error("Socket close failed: " + std::string(strerror(errno)));
    }
  }
}

int Socket::get_fd() const {
  return _fd;
}

/*
 * Destructor for Socket class
 */
Socket::~Socket() {
  /**************************************************/
  /* Close the socket                               */
  /**************************************************/
  Logger &logger = Logger::getInstance();

  if (close(_fd) == -1) {
    logger.log("Socket close failed: " + std::string(strerror(errno)));
  }
}
