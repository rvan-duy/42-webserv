#include <CGI.hpp>

/**************************************************/
/* Constructors / destructors		 	          */
/**************************************************/

CGI::CGI(std::string const& filePath): _filePath(filePath) {}

CGI::~CGI() {}

/**************************************************/
/* CGI methods						 	          */
/**************************************************/

int	CGI::executeFile(std::string *pDest) {
	int		fd[2];
	int		pid;

	Logger::getInstance().log("Starting CGI");

	
	return 0;
}