#pragma once

#include <string>
#include <iostream>

class Logger {
	public:
		static void log(std::string const& message) {
			std::string formattedMessage("\033[1;32m[LOG]:    \033[0m");
			formattedMessage.append(message);
			std::cout << formattedMessage << std::endl;
		}

		static void error(std::string const& message) {
			std::string formattedMessage("\033[1;31m[ERROR]:  \033[0m");
			formattedMessage.append(message);
			std::cerr << formattedMessage << std::endl;
		}
};
