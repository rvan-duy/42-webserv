#include "Server.hpp"

#include "Logger.hpp"

/*
 * Constructor for Server class
 */
Server::Server()
    : _port(-1),
      _maxBodySize(-1),
      _fd(-1),
      _domain(AF_INET6),
      _type(SOCK_STREAM),
      _defaultErrorPage(PageData(DEFAULT_ERROR_STATUS, DEFAULT_ERROR_PATH)),
      _accepted(-1),
      _host(PageData(DEFAULT_HOST_STATUS, DEFAULT_HOST_PATH)) {
  memset(&_servaddr, 0, sizeof(_servaddr));
  memset(&_buffer, 0, sizeof(_buffer));
}

/*
 * Destructor for Server class
 */
Server::~Server() {}

/**************************************************/
/* Getters                                        */
/**************************************************/

int Server::getFd() const {
  return _fd;
}

std::vector<std::string> Server::getServerName() const {
  return _serverName;
}

PageData Server::getHost() const {
  return _host;
}

PageData Server::getErrorPage() const {
  return _defaultErrorPage;
}

int Server::getPort() const {
  return _port;
}

int Server::getMaxBody() const {
  return _maxBodySize;
}

std::vector<Route> Server::getRoutes() const {
  return _routes;
}

std::vector<int> &Server::getConnectedClients() {
  return _connectedClients;
}

HttpRequest *Server::getRequestByDescriptor(int fd) {
  return _requests[fd];
}

std::string Server::getRoot(const std::string &uri) const {
  unsigned long maxlen = 0;
  std::string   root;
  for (std::vector<Route>::const_iterator it = _routes.begin(); it != _routes.end(); it++) {
    if (0 == uri.find(it->route)) {
      if (maxlen < it->route.size()) {
        root   = it->rootDirectory;
        maxlen = it->route.size();
      }
    }
  }
  return root;
}

/**************************************************/
/* Setters                                        */
/**************************************************/

int Server::setMaxBody(double const &value) {
  // TODO: check if 0 is possible
  if (value <= 0) {
    Logger::getInstance().error("Max body <= 0");
    return 1;
  } else if (value > INT_MAX) {
    Logger::getInstance().error("Max body > UINT_MAX");
    return 1;
  }
  _maxBodySize = value;
  return 0;
}

int Server::setHost(int const &statusCode, std::string const &filePath) {
  if (statusCode < 0) {
    Logger::getInstance().error("Incorrect statuscode set");
    return 1;
  }
  _host.statusCode = statusCode;
  _host.filePath   = filePath;
  return 0;
}

int Server::setErrorPage(int const &statusCode, std::string const &filePath) {
  if (statusCode < 0) {
    Logger::getInstance().error("Incorrect statuscode set");
    return 1;
  }
  _defaultErrorPage.statusCode = statusCode;
  _defaultErrorPage.filePath   = filePath;
  return 0;
}

void Server::setServerName(std::vector<std::string> const &value) {
  _serverName = value;
}

int Server::setPort(int const &value) {
  if (value > MAX_PORT) {
    Logger::getInstance().error("Port higher than MAX_PORT");
    return 1;
  } else if (value <= 0) {
    Logger::getInstance().error("Port higher than MAX_PORT");
    return 1;
  }
  _port = value;
  return 0;
}

void Server::addRoute(Route const &route) {
  _routes.push_back(route);
}

/**************************************************/
/* End of getters and setters                     */
/**************************************************/

/*
 * To check if variables are set
 */
// bool Server::hasServerName() const
// {
//     return (_serverName.size() != 0);
// }

// bool Server::hasPort() const
// {
//     return (_port != -1);
// }

// bool Server::hasMaxBody() const
// {
//     return (_maxBodySize != -1);
// }

// bool Server::hasRoutes() const
// {
//     return (_routes.size() != 0);
// }
