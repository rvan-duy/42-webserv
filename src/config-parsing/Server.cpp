#include <Server.hpp>

/**
 * Constructors / destructors
*/
Server::Server():
	_pDefaultErrorPage(NULL),
	_pHost(NULL),
	_port(-1),
	_maxBodySize(-1) {}

Server::~Server() {
	delete _pDefaultErrorPage;
	delete _pHost;
}

/**
 * Setters
*/
int	Server::setHost(int const& statusCode, std::string const& filePath) {
	if (statusCode < 0) {
		Logger::getInstance().error("Incorrect statuscode set");
		return 1;
	}
	_pHost = new PageData();
	_pHost->statusCode = statusCode;
	_pHost->filePath = filePath;
	return 0;
}

int		Server::setErrorPage(int const& statusCode, std::string const& filePath) {
	if (statusCode < 0) {
		Logger::getInstance().error("Incorrect statuscode set");
		return 1;
	}
	_pDefaultErrorPage = new PageData();
	_pDefaultErrorPage->statusCode = statusCode;
	_pDefaultErrorPage->filePath = filePath;
	return 0;
}

void	Server::setServerName(std::vector<std::string> value) {
	_serverName = value;
}

int		Server::setPort(int const& value) {
	if (value > MAX_PORT) {
		Logger::getInstance().error("Port higher than MAX_PORT");
		return 1;
	} else if (value <= 0) {
		Logger::getInstance().error("Port higher than MAX_PORT");
		return 1;
	}
	_port = value;
	return 0;
}

int		Server::setMaxBody(double const& value) {
	// TODO: check if 0 is possible
	if (value <= 0) {
		Logger::getInstance().error("Max body <= 0");
		return 1;
	} else if (value > INT_MAX) {
		Logger::getInstance().error("Max body > UINT_MAX");
		return 1;
	}
	_maxBodySize = value;
	return 0;
}

/**
 * To check if variables are set
*/
bool	Server::hasServerName() const {
	return (_serverName.size() == 0);
}

bool	Server::hasHost() const {
	return (_pHost != NULL);
}

bool	Server::hasPort() const {
	return (_port != -1);
}

bool	Server::hasMaxBody() const {
	return (_maxBodySize != -1);
}

bool	Server::hasErrorPage() const {
	return (_pDefaultErrorPage != NULL);
}

bool	Server::hasRoutes() const {
	return (_routes.size() != 0);
}

/**
 * Getters
*/
std::vector<std::string>	Server::getServerName() const {
	return _serverName;
}

PageData	&Server::getHost() const {
	return *_pHost;
}

PageData	&Server::getErrorPage() const {
	return *_pDefaultErrorPage;
}

int		Server::getPort() const {
	return _port;
}

int		Server::getMaxBody() const {
	return _maxBodySize;
}
