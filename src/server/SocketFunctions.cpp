#include <Server.hpp>

/*
 * Prepare the socket for incoming connections
 * @param backlog number of connections allowed on the incoming queue
 */
void Server::prepare(const int backlog)
{
    Logger &logger = Logger::getInstance();
    int on = 1; // used for setsockopt() and ioctl() calls
    
/*+ _serverName[0]*/ // this was included in log statement bust caused segf
    logger.log("[PREPARING] Server: Preparing :" + std::to_string(_port));

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
