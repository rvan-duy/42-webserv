#include "Server.hpp"
#include "Logger.hpp"

/*
 * Constructor for Server class
 */
Server::Server() : _host(DEFAULT_HOST), _maxBodySize(DEFAULT_MAX_BODY), _defaultErrorPage(PageData(DEFAULT_ERROR_STATUS, DEFAULT_ERROR_PATH))
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

std::string Server::getServerName() const
{
    return _serverName;
}

std::string Server::getHost() const
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

void Server::setServerName(std::string const &value)
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
        Logger::getInstance().error("Port invalid");
        return 1;
    }
    _port = value;
    return 0;
}

int Server::setHost(std::string const &host)
{
    if (_host != DEFAULT_HOST)
    {
        return 1;
    }
    _host = host;
    return 0;
}

void Server::addRoute(Route const &route)
{
    _routes.push_back(route);
}
