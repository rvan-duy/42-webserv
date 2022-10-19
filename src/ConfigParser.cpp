#include <ConfigParser.hpp>

/**
 * Constructors / destructors
*/
ConfigParser::ConfigParser(std::string const& filePath) {
	
}

ConfigParser::~ConfigParser() {
	
}

/**
 * File functions
*/
void	ConfigParser::openFile(std::string const& filePath) {
	_file.open("test");
}

/**
 * Getters / setters
*/
bool	ConfigParser::isFileOpen() const {
	return _file.is_open();
}
