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
    void addClient(const int &fd);
    void removeClient(const int socket);

    // Getters
    int getPort() const;
    int getFd() const;
    std::vector<Server> getServers() const;

    // Setters
    void addServer(Server const &server);
    int processRequest(int const &clientFd);

private:
    std::vector<Server> _servers;
    // key: client FD, value: request, server pair
    std::map<int, std::pair<HttpRequest *, Server *>> _clients;
    int _fd;
    int _port;
    int _accepted;
    struct sockaddr_in6 _servaddr;

    void _matchRequestToServer(int const &clientFd, HttpRequest *request);
    void _addRequestToClient(int const &clientFd, HttpRequest *request, Server *server);
    void _addBadRequestToClient(const int &fd, int type);
};