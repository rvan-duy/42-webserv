#include <Server.hpp>


/**
 * Constructors / destructors
*/
Server::Server():
	_serverName(NULL),
	_host(NULL),
	_port(NULL),
	_maxBodySize(NULL),
	_defaultErrorPage(NULL) {}

Server::~Server() {
	delete _serverName;
	delete _host;
	delete _defaultErrorPage;
	delete _port;
	delete _maxBodySize;
	// TODO: delete all routes
}

/**
 * Setters
*/
void	Server::setServerName(std::string const& value) {
	_serverName = new std::string(value);
}

void	Server::setHost(std::string const& value) {
	_host = new std::string(value);
}

void	Server::setErrorPage(std::string const& value) {
	_defaultErrorPage = new std::string(value);
}

int		Server::setPort(int const& value) {
	if (value > MAX_PORT) {
		Logger::getInstance().error("Port higher than MAX_PORT");
		return 1;
	} else if (value <= 0) {
		Logger::getInstance().error("Port higher than MAX_PORT");
		return 1;
	}
	_port = new int(value);
	return 0;
}

int		Server::setMaxBody(double const& value) {
	// TODO: check if 0 is possible
	if (value <= 0) {
		Logger::getInstance().error("Max body <= 0");
		return 1;
	} else if (value > UINT_MAX) {
		Logger::getInstance().error("Max body > UINT_MAX");
		return 1;
	}
	_maxBodySize = new unsigned int(value);
	return 0;
}

/**
 * To check if variables are set
*/
bool	Server::hasServerName() const {
	return (_serverName != NULL);
}

bool	Server::hasHost() const {
	return (_host != NULL);
}

bool	Server::hasPort() const {
	return (_port != NULL);
}

bool	Server::hasMaxBody() const {
	return (_maxBodySize != NULL);
}

bool	Server::hasErrorPage() const {
	return (_defaultErrorPage != NULL);
}

/**
 * Getters
*/
std::string		&Server::getServerName() const {
	return *_serverName;
}

std::string		&Server::getHost() const {
	return *_host;
}

std::string		&Server::getErrorPage() const {
	return *_defaultErrorPage;
}

int		&Server::getPort() const {
	return *_port;
}

unsigned int	&Server::getMaxBody() const {
	return *_maxBodySize;
}
