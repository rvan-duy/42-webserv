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
void Multiplexer::addServer(const Server &server, const short events)
{
    Logger &logger = Logger::getInstance();

    logger.log("[PREPARING] Multiplexer: Adding server to multiplexer: " + server.getServerName()[0] + ":" +
               std::to_string(server.getPort()));

    pollfd serverAsPollFD = {server.getFd(), events, 0};
    _servers.push_back(server);
    _clients.push_back(serverAsPollFD);

    logger.log("[PREPARING] Multiplexer: Server added to multiplexer, total number of stored sockets: " +
               std::to_string(_clients.size()));
}