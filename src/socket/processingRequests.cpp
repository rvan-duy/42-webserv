#include <Multiplexer.hpp>

/*
 * Reads data from a client and stores it in the buffer
 * @param result The string to store the data in
 * @param clientSocket The client socket to read from
 * @return The number of bytes read
 */
static int readFromClientFd(std::string *result, const int clientFd)
{
    Logger &logger = Logger::getInstance();
    const int bufferSize = 1000000; // 1MB buffer, is this enough?
    char buffer[bufferSize];
    bzero(buffer, bufferSize);

    // TODO: make loop?
    logger.log("[READING] Multiplexer: Reading data from client " + std::to_string(clientFd));
    int bytesReceived = read(clientFd, buffer, bufferSize);
    if (bytesReceived == -1)
    {
        logger.error("[READING] Multiplexer: Failed to read data from client " + std::to_string(clientFd) + ": " +
                     std::string(strerror(errno)));
        return -1;
    }
    *result = std::string(buffer, bytesReceived);
    logger.log("[READING] Multiplexer: Read " + std::to_string(bytesReceived) + " bytes from client " +
               std::to_string(clientFd));
    return bytesReceived;
}

/**
 * @return host without port OR if invalid host -> empty string
 */
static std::string getHostWithoutPort(HttpRequest *request)
{
    std::string host = request->getHeader("Host");
    if (host.length() == 0)
    {
        return "";
    }
    size_t semiCol = host.find(':');
    if (semiCol != std::string::npos)
    {
        host = host.substr(0, semiCol);
    }
    if (host == "localhost")
    {
        return "0.0.0.0";
    }
    return host;
}

/* Checks which servers are listening to the host from the request*/
static std::vector<Server> matchBasedOnHost(std::vector<Server> const &allServers, std::string const &host)
{
    std::vector<Server> listeningServers;

    for (std::vector<Server>::const_iterator it = allServers.begin(); it != allServers.end(); ++it)
    {
        if (it->getHost() == host)
        {
            listeningServers.push_back(*it);
        }
    }
    return listeningServers;
}

static Server matchBasedOnServerName(std::vector<Server> const &listeningServers, std::string const &serverName)
{
    for (std::vector<Server>::const_iterator it = listeningServers.begin(); it != listeningServers.end(); ++it)
    {
        if (it->getServerName() == serverName)
        {
            return *it;
        }
    }
    return listeningServers.at(0);
}

/**
 * Finds server that matches the sent request, then adds it to matching client
 */
void Socket::_matchRequestToServer(int const &clientFd, HttpRequest *request)
{
    std::string hostWithoutPort = getHostWithoutPort(request);
    std::vector<Server> listeningServers;
    Server result;

    if (hostWithoutPort.length() == 0)
    {
        return _addBadRequestToClient(clientFd, BAD_REQUEST);
    }
    /* Get servers that listen to the same ip address that the request is sent to*/
    listeningServers = matchBasedOnHost(_servers, hostWithoutPort);
    if (listeningServers.size() == 0)
    {
        return _addBadRequestToClient(clientFd, BAD_REQUEST);
    }
    else if (listeningServers.size() == 1)
    {
        return _addRequestToClient(clientFd, request, &listeningServers.at(0));
    }
    else
    {
        /* If multiple servers are listening to the ip address, get best match based on servername */
        result = matchBasedOnServerName(listeningServers, "");
        return _addRequestToClient(clientFd, request, &result);
    }
}

int Socket::processRequest(int const &clientFd)
{
    std::string rawRequest;
    HttpRequest *request;
    int bytesRead = 0;

    bytesRead = readFromClientFd(&rawRequest, clientFd);
    if (bytesRead <= 0)
    {
        _addBadRequestToClient(clientFd, INTERNAL_SERVER_ERROR);
        return 1;
    }
    else if (bytesRead == 0)
    {
        _addBadRequestToClient(clientFd, BAD_REQUEST);
        return 1;
    }
    request = RequestParser::parseHeader(rawRequest);
    _matchRequestToServer(clientFd, request);
    return 0;
}
