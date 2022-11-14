#pragma once

#include <string>
#include <Logger.hpp>

class CGI {
	public:
		CGI(std::string const& filePath);
		~CGI();

		int	executeFile(std::string *pDest);

	private:
		std::string _filePath;
};