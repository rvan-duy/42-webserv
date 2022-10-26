#include <Logger.hpp>

Logger* Logger::_logger = NULL;
/**
 * Constructors / destructors
*/
Logger::Logger() {
	std::string filePath(LOG_DEST);
	filePath.append("log.log");
	_file.open(filePath.c_str(), std::ios::out | std::ios::app);
	if (!isFileOpen()) {
		std::string errorMsg("Error: can't open log file: ");
		errorMsg.append(strerror(errno));
		Logger::error(errorMsg);
	} else {
		restart();
	}
}

Logger::~Logger() {
	if (isFileOpen()) {
		_file.close();
	}
}

/**
 * Logging functions
*/
void Logger::log(std::string const& message) {
	// To stdout
	std::string stdOutMessage("\033[1;32m[INFO]:   \033[0m");
	stdOutMessage.append(message);
	std::cout << stdOutMessage << std::endl;
	if (!isFileOpen()) {
		return ;
	}
	// To file
	std::string logMessage("[INFO]:   ");
	logMessage.append(message);
	_file << logMessage << std::endl;
}

void Logger::error(std::string const& message) {
	// To stdout
	std::string stdOutMessage("\033[1;31m[ERROR]  \033[0m");
	stdOutMessage.append(message);
	std::cerr << stdOutMessage << std::endl;
	if (!isFileOpen()) {
		return ;
	}
	// To file
	std::string logMessage("[ERROR]    ");
	logMessage.append(message);
	_file << logMessage << std::endl;
}

void Logger::restart(void) {
	if (!isFileOpen()) {
		return ;
	}
	std::string logMessage("[RESTART]\n");
	_file << logMessage << std::endl;
}

/**
 * Getters / setters
*/
Logger	&Logger::getInstance() {
	if (_logger == NULL) {
		_logger = new Logger();
		return *_logger;
	}
	return *_logger;
}

bool	Logger::isFileOpen() const {
	return _file.is_open();
}
