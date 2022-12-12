#include "Server.hpp"
#include "Logger.hpp"

/*
 * Constructor for Server class
 */
Server::Server() : _ipAddress(DEFAULT_IP_ADRESS), _maxBodySize(DEFAULT_MAX_BODY), _defaultErrorPage(PageData(DEFAULT_ERROR_STATUS, DEFAULT_ERROR_PATH)), _host(PageData(DEFAULT_HOST_STATUS, DEFAULT_HOST_PATH))
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

std::string Server::getIpAdress() const
{
    return _ipAddress;
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

HttpRequest *Server::getRequestByDescriptor(int fd)
{
    return _requests[fd];
}

HttpRequest *Server::getNextRequest() const
{
    return _unhandledRequests.at(0);
}

void Server::removeNextRequest()
{
    delete _unhandledRequests.at(0);
    _unhandledRequests.erase(_unhandledRequests.begin());
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
        Logger::getInstance().error("Port invalid");
        return 1;
    }
    _port = value;
    return 0;
}

int Server::setIpAddress(std::string const &address)
{
    if (_ipAddress != DEFAULT_IP_ADRESS)
    {
        return 1;
    }
    _ipAddress = address;
    return 0;
}

void Server::addRoute(Route const &route)
{
    _routes.push_back(route);
}

void Server::addRequest(HttpRequest *request)
{
    _unhandledRequests.push_back(request);
}
