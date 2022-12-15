#include <Socket.hpp>

/**
 * Constructors / destructors
 */

Socket::Socket(int const &port) : _fd(-1), _port(port), _accepted(-1)
{
    (void)_accepted;
    memset(&_servaddr, 0, sizeof(_servaddr));
}

Socket::~Socket()
{
}

/*
 * Prepare the socket for incoming connections
 * @param backlog number of connections allowed on the incoming queue
 */
void Socket::prepare()
{
    Logger &logger = Logger::getInstance();
    const int on = 1; // used for setsockopt() and ioctl() calls

    logger.log("[PREPARING] Socket: Preparing socket on " + std::to_string(_fd));

    /* Create an socket to receive incoming           */
    /* connections on                                 */
    _fd = socket(AF_INET6, SOCK_STREAM, 0);
    if (_fd == -1)
    {
        // logger.error("[PREPARING] Socket: Failed to create socket: " + std::string(strerror(errno)));   // double trouble?
        throw std::runtime_error("[PREPARING] Socket: Socket creation failed: " + std::string(strerror(errno)));
    }

    /* Set options on socket to reuse address         */
    if (setsockopt(_fd, SOL_SOCKET, SO_REUSEADDR, &on, sizeof(on)) == -1)
    {
        logger.error("[PREPARING] Socket: Failed to set socket options: " + std::string(strerror(errno)));
        throw std::runtime_error("[PREPARING] Socket: Socket options failed: " + std::string(strerror(errno)));
    }

    /* Set socket to be nonblocking. All of the sockets for      */
    /* the incoming connections will also be nonblocking since   */
    /* they will inherit that state from the listening socket.   */
    if (ioctl(_fd, FIONBIO, (char *)&on) == -1)
    {
        logger.error("[PREPARING] Socket: Failed to set socket to nonblocking: " + std::string(strerror(errno)));
        throw std::runtime_error("[PREPARING] Socket: Socket nonblocking failed: " + std::string(strerror(errno)));
    }

    /* Set all bytes in socket address structure to   */
    /* zero, and fill in the relevant data members    */
    memset(&_servaddr, 0, sizeof(_servaddr));
    _servaddr.sin6_family = AF_INET6;
    memcpy(&_servaddr.sin6_addr, &in6addr_any, sizeof(in6addr_any));
    _servaddr.sin6_port = htons(_port);

    /* Bind the address and port number to the socket */
    if (bind(_fd, (struct sockaddr *)&_servaddr, sizeof(_servaddr)) == -1)
    {
        logger.error("[PREPARING] Socket: Failed to bind socket: " + std::string(strerror(errno)));
        throw std::runtime_error("[PREPARING] Socket: Socket bind failed: " + std::string(strerror(errno)));
    }

    if (listen(_fd, BACKLOG) == -1)
    {
        logger.error("[PREPARING] Socket: Failed to prepare socket: " + std::string(strerror(errno)));
        throw std::runtime_error("[PREPARING] Socket: Socket listen failed: " + std::string(strerror(errno)));
    }
}

void Socket::sendResponse(const HttpResponse &response) const
{
    (void)response;
}

/*
 * Add a client socket to the server
 */
void Socket::addClient(const int &socket)
{
    Logger &logger = Logger::getInstance();

    logger.log("[POLLING] Socket: Adding client to socket");
    std::pair<HttpRequest *, Server *> newPair(nullptr, nullptr);
    _clients[socket] = newPair;
}

// TODO: fix segfault when printing request
void Socket::_addRequestToClient(int const &clientFd, HttpRequest *request, Server *server)
{
    std::pair<HttpRequest *, Server *> newPair(request, server);
    _clients[clientFd] = newPair;
}

void Socket::_addBadRequestToClient(const int &fd, int type)
{
    std::pair<HttpRequest *, Server *> newPair(nullptr, nullptr);
    // TODO: add bad request
    switch (type)
    {
    case INTERNAL_SERVER_ERROR:
        Logger::getInstance().error("Adding internal server error to client with fd: " + std::to_string(fd));
        // TODO: newPair.first = internal server error
        _clients[fd] = newPair;
        break;
    default:
        Logger::getInstance().error("Adding bad request to client with fd: " + std::to_string(fd));
        // TODO: newPair.first = bad request
        _clients[fd] = newPair;
        break;
    }
}

bool Socket::hasClient(const int &fd) const {
    return _clients.count(fd) != 0;
}

/*
 * Remove a client socket from the server
 */
void Socket::removeClient(const int socket)
{
    _clients.erase(socket);
}

int Socket::getPort() const
{
    return _port;
}

int Socket::getFd() const
{
    return _fd;
}

void Socket::addServer(Server const &server)
{
    _servers.push_back(server);
}

std::vector<Server> Socket::getServers() const
{
    return _servers;
}

Server& Socket::getServerForClient(const int clientFd) {
    // std::pair<*HttpRequest, *Server> tmp;
    // tmp = ;
    return *(_clients[clientFd].second);
}

HttpRequest* Socket::getRequestForClient(const int clientFd) {
    // std::pair<*HttpRequest, *Server> tmp;
    // tmp = ;
    return _clients[clientFd].first;
}