#include "Server.hpp"

#include "Logger.hpp"

/*
 * Constructor for Server class
 */
Server::Server() : _port(DEFAULT_PORT), _maxBodySize(-1), _fd(-1), _domain(AF_INET6), _type(SOCK_STREAM), _accepted(-1)
{
  memset(&_servaddr, 0, sizeof(_servaddr));
  memset(&_buffer, 0, sizeof(_buffer));
  memset(&_defaultErrorPage, 0, sizeof(_defaultErrorPage));
  memset(&_host, 0, sizeof(_host));
  _defaultErrorPage.statusCode = DEFAULT_ERROR_STATUS;
  _defaultErrorPage.filePath = DEFAULT_ERROR_PATH;
  _host.statusCode = DEFAULT_HOST_STATUS;
  _host.filePath = DEFAULT_HOST_PATH;
}

/*
 * Destructor for Server class
 */
Server::~Server()
{
  close(_fd);
}

/*
 * Prepare the socket for incoming connections
 * @param backlog number of connections allowed on the incoming queue
 */
void Server::prepare(const int backlog)
{
  Logger &logger = Logger::getInstance();
  int on = 1; // used for setsockopt() and ioctl() calls

  logger.log("[PREPARING] Server: Preparing " + _serverName[0] + ":" + std::to_string(_port));

  /**************************************************/
  /* Create an socket to receive incoming           */
  /* connections on                                 */
  /**************************************************/

  _fd = socket(_domain, _type, 0);
  if (_fd == -1)
  {
    logger.error("[PREPARING] Server: Failed to create socket: " + std::string(strerror(errno)));
    throw std::runtime_error("[PREPARING] Server: Socket creation failed: " + std::string(strerror(errno)));
  }

  /**************************************************/
  /* Set options on socket to reuse address         */
  /**************************************************/

  if (setsockopt(_fd, SOL_SOCKET, SO_REUSEADDR, &on, sizeof(on)) == -1)
  {
    logger.error("[PREPARING] Server: Failed to set socket options: " + std::string(strerror(errno)));
    throw std::runtime_error("[PREPARING] Server: Socket options failed: " + std::string(strerror(errno)));
  }

  /*************************************************************/
  /* Set socket to be nonblocking. All of the sockets for      */
  /* the incoming connections will also be nonblocking since   */
  /* they will inherit that state from the listening socket.   */
  /*************************************************************/

  if (ioctl(_fd, FIONBIO, (char *)&on) == -1)
  {
    logger.error("[PREPARING] Server: Failed to set socket to nonblocking: " + std::string(strerror(errno)));
    throw std::runtime_error("[PREPARING] Server: Socket nonblocking failed: " + std::string(strerror(errno)));
  }

  /**************************************************/
  /* Set all bytes in socket address structure to   */
  /* zero, and fill in the relevant data members    */
  /**************************************************/

  memset(&_servaddr, 0, sizeof(_servaddr));
  _servaddr.sin6_family = _domain;
  memcpy(&_servaddr.sin6_addr, &in6addr_any, sizeof(in6addr_any));
  _servaddr.sin6_port = htons(_port);

  /**************************************************/
  /* Bind the address and port number to the socket */
  /**************************************************/

  if (bind(_fd, (struct sockaddr *)&_servaddr, sizeof(_servaddr)) == -1)
  {
    logger.error("[PREPARING] Server: Failed to bind socket: " + std::string(strerror(errno)));
    throw std::runtime_error("[PREPARING] Server: Socket bind failed: " + std::string(strerror(errno)));
  }

  if (listen(_fd, backlog) == -1)
  {
    logger.error("[PREPARING] Server: Failed to prepare socket: " + std::string(strerror(errno)));
    throw std::runtime_error("[PREPARING] Server: Socket listen failed: " + std::string(strerror(errno)));
  }
}

/*
 * Add a client socket to the server
 */
void Server::addClient(const int socket)
{
  Logger &logger = Logger::getInstance();

  logger.log("[POLLING] Server: Adding client " + std::to_string(socket) + " to server " + _serverName[0] + ":" +
             std::to_string(_port));

  _connectedClients.push_back(socket);
}

/*
 * Remove a client socket from the server
 */
void Server::removeClient(const int socket)
{
  Logger &logger = Logger::getInstance();

  logger.log("[POLLING] Server: Removing client " + std::to_string(socket) + " from server " + _serverName[0] + ":" +
             std::to_string(_port));
  for (std::vector<int>::iterator it = _connectedClients.begin(); it != _connectedClients.end(); ++it)
  {
    if (*it == socket)
    {
      _connectedClients.erase(it);
      break;
    }
  }
}

/**************************************************/
/* Getters                                        */
/**************************************************/

int Server::getFd() const
{
  return _fd;
}

std::vector<std::string> Server::getServerName() const
{
  return _serverName;
}

PageData Server::getHost() const
{
  return _host;
}

PageData Server::getErrorPage() const
{
  return _defaultErrorPage;
}

int Server::getPort() const
{
  return _port;
}

int Server::getMaxBody() const
{
  return _maxBodySize;
}

std::vector<Route> Server::getRoutes() const
{
  return _routes;
}

/**************************************************/
/* Setters                                        */
/**************************************************/

int Server::setMaxBody(double const &value)
{
  // TODO: check if 0 is possible
  if (value <= 0)
  {
    Logger::getInstance().error("Max body <= 0");
    return 1;
  }
  else if (value > INT_MAX)
  {
    Logger::getInstance().error("Max body > UINT_MAX");
    return 1;
  }
  _maxBodySize = value;
  return 0;
}

int Server::setHost(int const &statusCode, std::string const &filePath)
{
  if (statusCode < 0)
  {
    Logger::getInstance().error("Incorrect statuscode set");
    return 1;
  }
  _host.statusCode = statusCode;
  _host.filePath = filePath;
  return 0;
}

int Server::setErrorPage(int const &statusCode, std::string const &filePath)
{
  if (statusCode < 0)
  {
    Logger::getInstance().error("Incorrect statuscode set");
    return 1;
  }
  _defaultErrorPage.statusCode = statusCode;
  _defaultErrorPage.filePath = filePath;
  return 0;
}

void Server::setServerName(std::vector<std::string> const &value)
{
  _serverName = value;
}

int Server::setPort(int const &value)
{
  if (value > MAX_PORT)
  {
    Logger::getInstance().error("Port higher than MAX_PORT");
    return 1;
  }
  else if (value <= 0)
  {
    Logger::getInstance().error("Port < 0 not allowed");
    return 1;
  }
  _port = value;
  return 0;
}

void Server::addRoute(Route const &route)
{
  _routes.push_back(route);
}

/**************************************************/
/* End of getters and setters                     */
/**************************************************/

/*
 * To check if variables are set
 */
bool Server::hasServerName() const
{
  return (_serverName.size() != 0);
}

bool Server::hasMaxBody() const
{
  return (_maxBodySize != -1);
}

bool Server::hasRoutes() const
{
  return (_routes.size() != 0);
}
