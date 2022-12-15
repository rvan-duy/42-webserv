#pragma once

#include <vector>
#include <arpa/inet.h>
#include <HttpResponse.hpp>

/* backlog number of connections allowed on the incoming queue */
#define BACKLOG 10

class Socket
{
public:
    Socket(int const &port);
    ~Socket();

    // Methods
    void prepare();
    void sendResponse(const HttpResponse &response) const;
    void addClient(const int fd);
    void removeClient(const int socket);

    // Getters
    int getPort() const;
    int getFd() const;
    std::vector<Server> getServers() const;

    // Setters
    void addServer(Server const &server);

private:
    std::vector<Server> _servers;
    // key: client FD, value: request, server pair
    std::map<int, std::pair<HttpRequest *, Server *>> _clients;
    // std::vector<int> _connectedClients;
    int _fd;
    int _port;
    int _accepted;
    struct sockaddr_in6 _servaddr;
};