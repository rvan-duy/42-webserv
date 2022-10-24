#include <string.h>
#include <fstream>
#include <Logger.hpp>

std::string	trimWhiteSpace(std::string line) {
	std::__1::string::iterator it =	line.begin();


}

void	loopOverFile(std::ifstream cFile) {
	std::string	line;

	while (std::getline(cFile, line)) {


		if (line[0] == '#' || line.empty())
                continue;
			
	}
}

int	parseConfigFile(std::string const& filePath) {
	Logger& logger = Logger::getInstance();
	std::ifstream cFile;

	cFile.open(filePath);
	if (!cFile.is_open()) {
		logger.error("Error opening file: " + std::string(strerror(errno)));
		return 1;
	}
	logger.log("Opened config file");

	cFile.close();
	return 0;
}
