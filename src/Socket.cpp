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
void Socket::addClient(const int socket)
{
    Logger &logger = Logger::getInstance();

    logger.log("[POLLING] Socket: Adding client " + std::to_string(socket) + " to server " + std::to_string(_fd) + ":" +
               std::to_string(_port));

    _connectedClients.push_back(socket);
}

/*
 * Remove a client socket from the server
 */
void Socket::removeClient(const int socket)
{
    Logger &logger = Logger::getInstance();

    logger.log("[POLLING] Socket: Removing client " + std::to_string(socket) + " from server " + std::to_string(_fd) + ":" +
               std::to_string(_port));
    for (std::vector<int>::iterator it = _connectedClients.begin(); it != _connectedClients.end(); ++it)
    {
        if (*it == socket)
        {
            _connectedClients.erase(it);
            break;
        }
    }
    // TODO: Should this be here?
    // _requests.erase(socket);
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

std::vector<int> Socket::getConnectedClients() const
{
    return _connectedClients;
}