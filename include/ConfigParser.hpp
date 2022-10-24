#pragma once

#include <iostream>
#include <exception>
#include <errno.h>
#include <fstream>
#include <Logger.hpp>

class ConfigParser {
	public:
		ConfigParser(std::string const& filePath);
		~ConfigParser();

		class FileOpenException: public std::exception {
			public:
				char const *what() const throw() {
					// std::string message("Error opening file: ");
					return strerror(errno);
				}
		};

	private:
		std::ifstream	_file;
		
		bool	isFileOpen() const;
		void	openFile(std::string const& filePath);
};
