#include "Server.hpp"
#include "Logger.hpp"

/*
 * Constructor for Server class
 */
Server::Server() : _port(-1), _maxBodySize(-1), _defaultErrorPage(PageData(DEFAULT_ERROR_STATUS, DEFAULT_ERROR_PATH)), _host(PageData(DEFAULT_HOST_STATUS, DEFAULT_HOST_PATH))
{
}

/*
 * Destructor for Server class
 */
Server::~Server()
{
}

/**************************************************/
/* Getters                                        */
/**************************************************/

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

HttpRequest *Server::getRequestByDiscriptor(int fd)
{
    return _requests[fd];
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
        Logger::getInstance().error("Port higher than MAX_PORT");
        return 1;
    }
    _port = value;
    return 0;
}

void Server::addRoute(Route const &route)
{
    _routes.push_back(route);
}
