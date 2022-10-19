#pragma once

/**
 * Log levels explained:
 * Levels go from less verbose to more verbose
 *	- 0: Production: for regular use
			Basically only used to log main functions and errors
	- 1: Testing: for during development
			Log general events to get an idea of the flow of the program
	- 2: Debug: for when it's unclear what is going wrong
			Tons 'o loggin' for when you don't know what the hell is going on
*/

# include <string.h>
# include <sys/errno.h>
#include <iostream>
#include <sys/stat.h>
#include <fstream>

# define LOG_DEST "../../logs"

enum ELogLevel {
	PROD,
	DEV,
	DEBUG
};

#ifndef LOG_LEVEL
# define LOG_LEVEL 1
#endif

/**
 *	Singleton class.
 *	Only one logger can be available in the program
 *	Usage: 
 *	 	Logger* logger = Logger::GetInstance();
 *		logger->log("Hi");
*/
class Logger {
	public:
		static Logger &GetInstance();

		void	log(std::string const& text);
		void	error(std::string const& text);

		/* Thrown when failed to open file */
		class FileError: public std::exception {
			public:
				char const *what() const throw() {
					return strerror(errno);
				}
		};


	private:
		static Logger*	_Logger;
		std::ofstream	_logFile;

		void	openFile();
		bool	isFileOpen() const;

	protected:
		Logger();
		~Logger();
};
