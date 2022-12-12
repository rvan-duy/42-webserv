#include <Multiplexer.hpp>

/*
 * Reads data from a client and stores it in the buffer
 * @param clientSocket The client socket to read from
 * @param result The string to store the data in
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

/**
 * TODO: rewrite this
 */
static std::vector<Server> matchBasedOnIp(std::vector<Server> &allServers, std::string const &ipAdress)
{
    std::vector<Server> listeningServers;

    for (std::vector<Server>::iterator it = allServers.begin(); it != allServers.end(); ++it)
    {
        if (it->getIpAdress() == ipAdress)
        {
            listeningServers.push_back(*it);
        }
    }
    return listeningServers;
}

/**
 * TODO: fix this
 */
static Server matchBasedOnName(std::vector<Server> &servers, std::string const &serverName)
{
}

/**
 * Legacy:
 */
// void Multiplexer::matchRequestToServer(HttpRequest *request, int const &fd)
// {
//     std::vector<Server> listeningServers = matchBasedOnPort(_servers, fd);
//     if (listeningServers.size() == 0)
//         throw "No valid servers";
//     std::string host = request->getHeader("Host");
//     /* If no host is specified, go for default server */
//     if (host == "")
//     {
//         listeningServers.at(0).addRequest(request);
//         return;
//     }
//     listeningServers = matchBasedOnIp(listeningServers, "hi");
//     if (listeningServers.size() == 1)
//     {
//         listeningServers.at(0).addRequest(request);
//         return;
//     }
//     Server match = matchBasedOnName(listeningServers, host);
// }

int Multiplexer::_processRequest(int const &fd)
{
    std::string rawRequest;
    HttpRequest *request;
    int readStatus = 0;

    readStatus = readFromSocket(&rawRequest, fd);
    if (readStatus < 0)
        return 1;
    else if (readStatus == 0)
    {
        // Return ok response?
        return 2;
    }
    request = RequestParser::parseHeader(rawRequest);
    if (request == NULL)
    {
        // Return bad request
        return 1;
    }
    // TODO: match request to correct virtual server
    return 0;
}
