#pragma once

#include <string>
#include <iostream>
#include <fstream>
#include <errno.h>

#define LOG_DEST "logs/"

/**
 * Singleton logger
 * Will automatically open file in LOG_DEST and append to it
 * Prints to file in LOG_DEST and stdout / stderr
 * Usage:
 * 		Logger& logger = Logger::getInstance();
 * 		logger.log("example log");
*/
class Logger {
	public:
		// Retrieves the logger instance
		static Logger	&getInstance();

		void log(std::string const& message);
		void error(std::string const& message);
	
	protected:
		// Constructors are protected since they shouldn't be called by any other class or file
		Logger();
		~Logger();

		// Singleton, this is the actual instance of the logger
		static Logger* _logger;

	private:
		// Logging file
		std::ofstream	_file;
		// Gets if file is open
		bool	isFileOpen() const;

		// Prints restart message to log
		void	restart();
};
