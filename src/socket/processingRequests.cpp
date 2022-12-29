#include <Multiplexer.hpp>

#define MAX_REQUEST_SIZE 1000000
#define BUFFER_SIZE 10

static int readFromClientFd(std::string *result, const int clientFd) {
  char buffer[BUFFER_SIZE + 1];
  memset(buffer, 0, BUFFER_SIZE + 1);

  ssize_t bytesReceived = read(clientFd, buffer, BUFFER_SIZE);
  if (bytesReceived == -1) {
    Logger::getInstance().error(
        "[SOCKET]: read failed: " + std::to_string(clientFd) + ": " +
        std::string(strerror(errno)));
    return -1;
  }
  *result = std::string(buffer, bytesReceived);
  Logger::getInstance().log(
      "[READING] Multiplexer: Read " + std::to_string(bytesReceived), VERBOSE);
  return bytesReceived;
}

/**
 * @return host without port OR if invalid host -> empty string
 */
static std::string getHostWithoutPort(HttpRequest *request) {
  std::string host = request->getHeader("Host");
  if (host.length() == 0) {
    return "";
  }
  size_t semiCol = host.find(':');
  if (semiCol != std::string::npos) {
    host = host.substr(0, semiCol);
  }
  return host;
}

/* Checks which servers are listening to the host from the request*/
static Server &matchBasedOnHost(std::vector<Server> &allServers,
                                std::string const &host) {
  for (std::vector<Server>::iterator it = allServers.begin();
       it != allServers.end(); ++it) {
    if (it->getServerName() == host) {
      return *it;
    }
  }
  return allServers.front();
}

/**
 * Finds server that matches the sent request, then adds it to matching client
 */
void Socket::_matchRequestToServer(int const &clientFd, HttpRequest *request) {
  std::string hostWithoutPort = getHostWithoutPort(request);
  Server &result = matchBasedOnHost(_servers, hostWithoutPort);
  return _addRequestToClient(clientFd, request, &result);
}

int Socket::processRequest(int const &clientFd) {
  Logger &logger = Logger::getInstance();
  std::string rawRequest;
  HttpRequest *request;
  int bytesRead = 0;

  logger.log("Starting to read from client", VERBOSE);
  bytesRead = readFromClientFd(&rawRequest, clientFd);
  if (bytesRead < 0) {
    _addBadRequestToClient(clientFd, HTTPStatusCode::INTERNAL_SERVER_ERROR);
    return 1;
  } else if (bytesRead == 0) {
    _addBadRequestToClient(clientFd, HTTPStatusCode::BAD_REQUEST);
    return 1;
  }

  if (isChunked(clientFd)) {
    request = RequestParser::processChunk(rawRequest);
    addChunk(request, clientFd);
    delete request;  // the body or header data has been added to og request
    return 0;
  }
  request = RequestParser::parseHeader(rawRequest);
  if (request->isFirstChunk())  // trailing headers will be caught by above
                                // ischunked already
  {
    request->unChunkBody();
  }
  _matchRequestToServer(clientFd, request);
  return 0;
}
