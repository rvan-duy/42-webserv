#pragma once

#include <string>
#include <Logger.hpp>
#include <errno.h>
#include <unistd.h>
#include <fstream>
#include <sys/stat.h>
#include <sys/wait.h>

/* Piping and forking variables */
#define CHILD 0
#define READ 0
#define WRITE 1
#define BUFFER_SIZE 1024

#define PATH_TO_PYTHON "/usr/bin/python"

class CGI {
	public:
		CGI(std::string const& rootDir, char *const *env);
		~CGI();

		int	executeFile(std::string *pDest, std::string const& filePath, std::string const& body) const;

	private:
		std::string _rootDir;
		char *const *_env;

		int	forkCgiFile(int fd[2], std::string const& filePath, std::string const& body) const;
};
