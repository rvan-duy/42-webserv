#include "Multiplexer.hpp"

#include "Logger.hpp"
#include <algorithm>

Multiplexer::Multiplexer() : _endServer(false) {}

Multiplexer::~Multiplexer() {}

/*
 * Add a server to the multiplexer
 * @param fd The file descriptor of the server
 * @param events The events to listen for
 */
void Multiplexer::addServer(const Server &server, const short events) {
  Logger &logger = Logger::getInstance();

  logger.log("[PREPARING] Multiplexer: Adding server to multiplexer: " + server.getServerName()[0] + ":" +
             std::to_string(server.getPort()));

  pollfd serverAsPollFD = {server.getFd(), events, 0};
  _servers.push_back(server);
  _clients.push_back(serverAsPollFD);

  logger.log("[PREPARING] Multiplexer: Server added to multiplexer, total number of stored sockets: " +
             std::to_string(_clients.size()));
}

void  pollfd_debug_helper(pollfd *arr, int len)
{
  Logger &logger = Logger::getInstance();
  
  for (int i = 0; i < len; i++)
  {
    logger.debug("socket : " + std::to_string(arr[i].fd));
    logger.debug("event : " + std::to_string(arr[i].events));
    logger.debug("revents : " + std::to_string(arr[i].revents));
  }
}

/*
 * Wait for events on the clients
 * @param timeout Timeout in milliseconds, -1 for infinite
 */
void Multiplexer::waitForEvents(const int timeout) {
  Logger &logger = Logger::getInstance();
  static int quiter;
  std::vector<int> markForRemoval;

  logger.log("[POLLING] Multiplexer: Starting poll() loop with timeout of " + std::to_string(timeout) +
             " milliseconds");
  do {
    if (_pollSockets(timeout) == -1) break;
    logger.log("[POLLING] Multiplexer: Poll returned successfully, checking for events on sockets");
    const int NUMBER_OF_SOCKETS_TO_CHECK = _clients.size();
    for (int i = 0; i < NUMBER_OF_SOCKETS_TO_CHECK; i++) {
      const int EVENT_TYPE    = _getEvent(_clients[i]);
      const int CLIENT_SOCKET = _clients[i].fd;

      switch (EVENT_TYPE) {
        case POLLIN: {
          if (_isServer(CLIENT_SOCKET)) {
            _addClient(CLIENT_SOCKET);
          } else {
            std::string rawRequest;
            if (_readData(CLIENT_SOCKET, rawRequest) == 0) {
              markForRemoval.push_back(CLIENT_SOCKET);
            } else {
              _getServerForClient(CLIENT_SOCKET).buildRequest(rawRequest, CLIENT_SOCKET);
              // - Check if request->serverName is a valid server that we have
              _clients[i].revents = POLLOUT;
            }
          }
          break;
        }

        case POLLOUT: {
          std::string tmp_index("index.html");
          Server  client_server = _getServerForClient(CLIENT_SOCKET);
          HttpRequest *client_request = client_server.getRequestByDiscriptor(CLIENT_SOCKET);
          if (client_request)
          {
            HttpResponse client_response = client_request->constructResponse(client_server, tmp_index); // index.html shouldnt be hardcoded..
            send(CLIENT_SOCKET, (void *)client_response.toStr().c_str(), client_response.toStr().size(), 0);
          }
          delete client_request;
          markForRemoval.push_back(CLIENT_SOCKET);
          break;
        }

        // case POLLOUT, POLLERR, POLLHUP?
        case POLLNVAL: {
          logger.debug("POLLNVAL on discriptor : " + std::to_string(CLIENT_SOCKET));
          break;
        }

        default: {
          logger.log("[POLLING] Multiplexer: No events on socket");
          break;
        }
      }
    }
    for (size_t i = 0; i < markForRemoval.size(); i++)
      _removeClient(markForRemoval[i]);
    markForRemoval.clear();
  quiter++;
  if (quiter == 8)
    _endServer = true;
  } while (_endServer == false);
}

/*
 * Send data to a client
 * @param clientSocket The socket of the client
 * @param data The data to send
 * @return The number of bytes send
 */
int Multiplexer::_sendData(const int socket, const std::string &data) const {
  Logger &logger = Logger::getInstance();

  logger.log("[WRITING] Multiplexer: Writing data to client: \n" + data);
  return write(socket, data.c_str(), data.length());
}

/*
 * Check if a fd is a server
 * @param fd The file descriptor to check
 * @return True if the fd is a server, false if not
 */
bool Multiplexer::_isServer(const int fd) const {
  Logger &logger = Logger::getInstance();

  for (std::vector<Server>::const_iterator it = _servers.begin(); it != _servers.end(); ++it) {
    if (it->getFd() == fd) {
      logger.log("[POLLING] Multiplexer: Socket is a server [" + it->getServerName()[0] + ":" +
                 std::to_string(it->getPort()) + "]");
      return true;
    }
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
  pollfd client = {newSocket, POLLIN | POLLOUT, 0};
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
 * Reads data from a client and stores it in the buffer
 * @param clientSocket The client socket to read from
 * @param result The string to store the data in
 * @return The number of bytes read
 */
int Multiplexer::_readData(const int socket, std::string &result) const {
  Logger   &logger      = Logger::getInstance();
  const int BUFFER_SIZE = 1000000;  // 1MB buffer, is this enough?
  char      buffer[BUFFER_SIZE];
  bzero(buffer, BUFFER_SIZE);

  logger.log("[READING] Multiplexer: Reading data from socket " + std::to_string(socket));
  int bytesReceived = read(socket, buffer, BUFFER_SIZE);
  if (bytesReceived == -1) {
    logger.error("[READING] Multiplexer: Failed to read data from socket " + std::to_string(socket) + ": " +
                 std::string(strerror(errno)));
    return -1;
  }
  result = std::string(buffer, bytesReceived);
  logger.log("[READING] Multiplexer: Read " + std::to_string(bytesReceived) + " bytes from socket " +
             std::to_string(socket));
  return bytesReceived;
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
  } else if (fd.revents & POLLERR) {
    logger.log("[POLLING] Multiplexer: POLLERR event on socket " + std::to_string(fd.fd));
    return POLLERR;
  } else if (fd.revents & POLLHUP) {
    logger.log("[POLLING] Multiplexer: POLLHUP event on socket " + std::to_string(fd.fd));
    return POLLHUP;
  } else if (fd.revents & POLLNVAL) {
    logger.log("[POLLING] Multiplexer: POLLNVAL event on socket " + std::to_string(fd.fd));
    return POLLNVAL;
  } else {
    logger.log("[POLLING] Multiplexer: Unknown event on socket " + std::to_string(fd.fd));
    return -1;
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

  // pollfd_debug_helper(fds, _clients.size());

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

Server& Multiplexer::_getServerForClient(int fd) {
  std::vector<Server>::iterator it;
  std::vector<int>  connectedClients;
  for (it = _servers.begin(); it != _servers.end(); it++) {
    if (std::find(it->getConnectedClients().begin(),
      it->getConnectedClients().end(), fd) != it->getConnectedClients().end())
        return (*it);
  }
  return (*it);
}