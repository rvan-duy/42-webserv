#include <ConfigParser.hpp>

/**
 * Constructors / destructors
*/
ConfigParser::ConfigParser(std::string const& filePath) {
	Logger& logger = Logger::getInstance();
	try {
		openFile(filePath);
	} catch (std::exception &e) {
		logger.error("Error opening file: " + std::string(e.what()));
		throw e;
	}
	logger.log("Opened config file");
}

ConfigParser::~ConfigParser() {
	if (isFileOpen()) {
		_file.close();
	}
}

/**
 * File functions
*/
void	ConfigParser::openFile(std::string const& filePath) {
	_file.open(filePath);
	if (!isFileOpen()) {
		throw FileOpenException();
	}
}

/**
 * Getters / setters
*/
bool	ConfigParser::isFileOpen() const {
	return _file.is_open();
}
