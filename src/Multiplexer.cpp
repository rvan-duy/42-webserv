#include "Multiplexer.hpp"

#include "Logger.hpp"

Multiplexer::Multiplexer() : _endServer(false) {
  memset(&_buffer, 0, sizeof(_buffer));
}

Multiplexer::~Multiplexer() {}

/*
 * Add a server to the multiplexer
 * @param fd The file descriptor of the server
 * @param events The events to listen for
 */
void Multiplexer::addServer(const Server &server, const short events) {
  Logger &logger = Logger::getInstance();

  // TODO: Can a server have multiple names????????? for now lets add [0]
  logger.log("[PREPARING] Multiplexer: Adding server to multiplexer: " + server.getServerName()[0] + ":" +
             std::to_string(server.getPort()));

  pollfd serverAsPollFD = {server.getFd(), events, 0};
  _servers.push_back(server);
  _clients.push_back(serverAsPollFD);

  logger.log("[PREPARING] Multiplexer: Server added to multiplexer, total number of stored sockets: " +
             std::to_string(_clients.size()));
}

/*
 * Wait for events on the clients
 * @param timeout Timeout in milliseconds, -1 for infinite
 */
void Multiplexer::waitForEvents(const int timeout) {
  Logger &logger = Logger::getInstance();

  logger.log("[POLLING] Multiplexer: Starting poll() loop with timeout of " + std::to_string(timeout) + " milliseconds");
  do {
    if (_pollSockets(timeout) == -1) break;

    /**************************************************/
    /* Poll has successfully returned, this means a   */
    /* socket has an event to handle.                 */
    /**************************************************/

    logger.log("[POLLING] Multiplexer: Poll returned successfully, checking for events on sockets");
    const int NUMBER_OF_SOCKETS_TO_CHECK = _clients.size();
    for (int i = 0; i < NUMBER_OF_SOCKETS_TO_CHECK; i++) {  // Loop through all sockets inside the multiplexer
      int eventType = _getEvent(_clients[i]);               // Logging is done inside the _getEvent() method

      /**************************************************/
      /* Main switch to handle the different events     */
      /**************************************************/
      switch (eventType) {
        /**************************************************/
        /* No event                                       */
        /**************************************************/
        case 0: {
          break;
        }

        /**************************************************/
        /* Read event                                     */
        /**************************************************/
        case POLLIN: {
          if (_isServer(_clients[i].fd)) {                 // If the socket is a server
            _addClient(_clients[i].fd);                    // Accept it as a client
          } else {                                         // If not, this means it is a client
            std::string data = _readData(_clients[i].fd);  // Read the data
            if (data.empty()) {                            // If the data is empty, the client has disconnected
              _removeClient(_clients[i].fd);               // Remove the client from the multiplexer
            } else {                                       // If the data is not empty, the client has sent data
              logger.log("[POLLING] Multiplexer: Received data from client: \n" + data);  // Log the data
              
              // it just sends back the data it received for testing purposes
              send(_clients[i].fd, data.c_str(), data.size(), 0);
              
              _removeClient(_clients[i].fd);  // Remove the client from the multiplexer
            }
          }
          break;
        }

        // case POLLOUT, POLLERR, POLLHUP?

        /**************************************************/
        /* Default event                                  */
        /**************************************************/
        default: {
          logger.error("[POLLING] Multiplexer: CODE NOT IMPLEMENTED YET");
          break;
        }
      }
    }

  } while (_endServer == false);
}

/*
 * Check if a fd is a server
 * @param fd The file descriptor to check
 * @return True if the fd is a server, false if not
 */
bool Multiplexer::_isServer(const int fd) const {
  for (std::vector<Server>::const_iterator it = _servers.begin(); it != _servers.end(); ++it) {
    if (it->getFd() == fd) return true;
  }
  return false;
}

/*
 * Get a server from a fd
 * @param fd The file descriptor to get the server from
 * @return The server
 */
Server Multiplexer::_getServer(const int fd) const {
  for (std::vector<Server>::const_iterator it = _servers.begin(); it != _servers.end(); ++it) {
    if (it->getFd() == fd) return *it;
  }
  return Server();
}

/*
 * Add a client socket to the multiplexer, so we can listen for events on it
 * @param socket The socket to add to the multiplexer
 */
void Multiplexer::_addClient(const int socket) {
  Logger &logger = Logger::getInstance();

  logger.log("[POLLING] Multiplexer: Adding server socket " + std::to_string(socket) + " to multiplexer");
  int newSocket = accept(socket, nullptr, nullptr);
  if (newSocket == -1) {
    logger.error("[POLLING] Multiplexer: Failed to accept new connection: " + std::string(strerror(errno)));
    _endServer = true;
    return;
  }
  logger.log("[POLLING] Multiplexer: New connection accepted: " + std::to_string(newSocket));
  pollfd client = {newSocket, POLLIN, 0};
  _clients.push_back(client);
  for (std::vector<Server>::iterator it = _servers.begin(); it != _servers.end(); ++it) {
    if (it->getFd() == socket) {
      it->addClient(newSocket);
      break;
    }
  }
  logger.log("[POLLING] Multiplexer: New connection added to multiplexer, total number of stored sockets: " +
             std::to_string(_clients.size()));
}

/*
 * Remove a client socket from the multiplexer
 * @param socket The socket to remove from the multiplexer
 */
void Multiplexer::_removeClient(const int socket) {
  Logger &logger = Logger::getInstance();

  logger.log("[POLLING] Multiplexer: Removing client socket " + std::to_string(socket) + " from multiplexer");
  for (std::vector<pollfd>::iterator it = _clients.begin(); it != _clients.end(); ++it) {
    if (it->fd == socket) {
      close(socket);
      _clients.erase(it);
      logger.log("[POLLING] Multiplexer: Client socket " + std::to_string(socket) + " removed from multiplexer");
      break;
    }
  }
  for (std::vector<Server>::iterator it = _servers.begin(); it != _servers.end(); ++it) {
    it->removeClient(socket);
  }
}

/*
 * Reads data from a socket and returns it as a string
 * @param socket The socket to read from
 */
std::string Multiplexer::_readData(const int socket) {
  Logger &logger = Logger::getInstance();

  logger.log("[POLLING] Multiplexer: Reading data from socket " + std::to_string(socket));
  int bytesRead = read(socket, _buffer, sizeof(_buffer));
  if (bytesRead == -1) {
    logger.error("[POLLING] Multiplexer: Failed to read data from socket " + std::to_string(socket));
    _endServer = true;
    return "";
  } else if (bytesRead == 0) {
    logger.log("[POLLING] Multiplexer: Connection closed on socket " + std::to_string(socket));
    logger.error("[POLLING] Multiplexer: CODE NOT IMPLEMENTED YET");
    return "";
  }
  logger.log("[POLLING] Multiplexer: Read " + std::to_string(bytesRead) + " bytes from socket " + std::to_string(socket));
  return std::string(_buffer, bytesRead);
}

/*
 * Check for an return event on a socket
 * @param fd The pollfd to check
 */
int Multiplexer::_getEvent(const pollfd &fd) {
  Logger &logger = Logger::getInstance();

  logger.log("[POLLING] Multiplexer: Checking for event on socket " + std::to_string(fd.fd));
  if (fd.revents == 0) {
    logger.log("[POLLING] Multiplexer: No event on socket " + std::to_string(fd.fd));
    return 0;
  } else if (fd.revents & POLLIN) {
    logger.log("[POLLING] Multiplexer: POLLIN event on socket " + std::to_string(fd.fd));
    return POLLIN;
  } else if (fd.revents & POLLOUT) {
    logger.log("[POLLING] Multiplexer: POLLOUT event on socket " + std::to_string(fd.fd));
    return POLLOUT;
  } else {
    logger.log("[POLLING] Multiplexer: Unknown event on socket " + std::to_string(fd.fd));
    return POLLERR;
  }
}

/*
 * Poll the sockets
 * @param timeout Timeout in milliseconds, -1 for infinite
 * @return 0 on success, -1 on error
 */
int Multiplexer::_pollSockets(const int timeout) {
  Logger &logger = Logger::getInstance();
  pollfd *fds    = &_clients[0];

  logger.log("[POLLING] Multiplexer: Polling for events on " + std::to_string(_clients.size()) + " sockets...");
  int pollResult = poll(fds, _clients.size(), timeout);
  switch (pollResult) {
    case -1:
      logger.error("[POLLING] Multiplexer: poll() returned -1, error occurred: " + std::string(strerror(errno)));
      return -1;
    case 0:
      logger.log("[POLLING] Multiplexer: poll() returned 0, timeout occurred");
      return -1;
    default:
      logger.log("[POLLING] Multiplexer: " + std::to_string(_clients.size()) + " sockets are ready");
      return 0;
  }
}
