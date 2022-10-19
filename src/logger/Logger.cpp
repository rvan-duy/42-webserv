#include <Logger.hpp>

Logger*	Logger::_Logger = NULL;
/**
 * Constructors / destructors
*/
Logger::Logger() {
	if (LOG_LEVEL > 0) {
		openFile();
	}
	std::cout << "Logger constructor called" << std::endl;
}

Logger::~Logger() {
	if (isFileOpen()) {
		_logFile.close();
	}
}

/**
 * Logging
*/
void	Logger::log(std::string const& text) {
	if (isFileOpen()) {
		_logFile << text << std::endl;
	}
}

void	Logger::error(std::string const& text) {
	if (isFileOpen()) {
		_logFile << text << std::endl;
	}
}

/**
 *  Other functions
*/
void	Logger::openFile() {
	std::string logDest = std::string(LOG_DEST);
	logDest.append("/log1.log");
	std::cout << logDest << std::endl;
	try {
		_logFile = std::ofstream("logDest");
	} catch (std::exception &e) {
		std::cout << "Logger: error opening file: " << e.what() << std::endl;
	}
}

Logger &Logger::GetInstance() {
	if (_Logger == NULL) {
		_Logger = new Logger();
	}
	return *_Logger;
}

bool	Logger::isFileOpen() const {
	return _logFile.is_open();
}
