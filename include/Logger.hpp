#pragma once

#include <string>
#include <iostream>
#include <fstream>
#include <errno.h>
#include <ctime>
#include <unistd.h>
#include <stdio.h>

#define LOG_DEST "logs/"

enum ELogLevel {
	INFO,
	ERROR
};

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
		pid_t			_parentPid;
		// Gets if file is open
		bool	isFileOpen() const;

		std::string	getTimeStamp() const;
		std::string getPid() const;
		void	restart();

		void logToConsole(std::string const& levelMsg, ELogLevel level, std::string const& message);
		void logToFile(std::string const& levelMsg, std::string const& message);
};
