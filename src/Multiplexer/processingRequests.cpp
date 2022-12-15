#include <Multiplexer.hpp>

/*
 * Reads data from a client and stores it in the buffer
 * @param result The string to store the data in
 * @param clientSocket The client socket to read from
 * @return The number of bytes read
 */
static int readFromSocket(std::string *result, const int socket)
{
    Logger &logger = Logger::getInstance();
    const int bufferSize = 1000000; // 1MB buffer, is this enough?
    char buffer[bufferSize];
    bzero(buffer, bufferSize);

    // TODO: make loop?
    logger.log("[READING] Multiplexer: Reading data from socket " + std::to_string(socket));
    int bytesReceived = read(socket, buffer, bufferSize);
    if (bytesReceived == -1)
    {
        logger.error("[READING] Multiplexer: Failed to read data from socket " + std::to_string(socket) + ": " +
                     std::string(strerror(errno)));
        return -1;
    }
    *result = std::string(buffer, bytesReceived);
    logger.log("[READING] Multiplexer: Read " + std::to_string(bytesReceived) + " bytes from socket " +
               std::to_string(socket));
    return bytesReceived;
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

static int matchRequestToServer(std::vector<Server> const &allServers, HttpRequest *request)
{
    std::vector<Server> listeningServers;
    std::string host = request->getHeader("Host");
    Server result;

    /* Get servers that listen to the same ip address that the request is sent to*/
    listeningServers = matchBasedOnHost(allServers, request->getUrl());
    if (listeningServers.size() == 0)
    {
        return BAD_REQUEST;
    }
    else if (listeningServers.size() == 1)
    {
        result = listeningServers.at(0);
    }
    else
    {
        /* If multiple servers are listening to the ip address, get best match based on servername */
        result = matchBasedOnServerName(listeningServers, "");
    }
    result.addRequest(request);
    return 0;
}

int Multiplexer::_processRequest(int const &clientFd, Socket &socket)
{
    std::string rawRequest;
    HttpRequest *request;
    int bytesRead = 0;

    bytesRead = readFromSocket(&rawRequest, clientFd);
    if (bytesRead < 0)
    {
        return INTERNAL_SERVER_ERROR;
    }
    else if (bytesRead == 0)
    {
        return OK;
    }
    request = RequestParser::parseHeader(rawRequest);
    if (request == NULL)
    {
        return BAD_REQUEST;
    }
    // If no host is specified -> bad request
    if (request->getHeader("Host").length() == 0)
    {
        return BAD_REQUEST;
    }
    // Server& server = matchRequestToServer(socket.getServers(), request);

    return 0;
}
