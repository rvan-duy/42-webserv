#pragma once

#include <errno.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <unistd.h>

#include <Logger.hpp>
#include <StatusCodes.hpp>
#include <Webserver.hpp>
#include <fstream>
#include <string>

/* Piping and forking variables */
#define CHILD 0
#define READ 0
#define WRITE 1
#define BUFFER_SIZE 1024

#define PATH_TO_PYTHON "/usr/bin/python3"
#define PATH_TO_PHP "/opt/homebrew/Cellar/php/8.1.13/bin/php"

class CGI {
 public:
  CGI(char *const *env);
  ~CGI();

  HTTPStatusCode executeFile(std::string *pBody,
                             std::vector<std::string> *pHeaders,
                             std::string const &filePath,
                             std::string const &body) const;

 protected:
  /* Don't change the order of the '*' and keywords here please */
  char *const *_env;

  int forkCgiFile(int fd[2], std::string const &filePath,
                  std::string const &body) const;
};
