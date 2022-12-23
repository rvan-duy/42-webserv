#ifndef WEBSERVER_HPP
#define WEBSERVER_HPP

#include <string>
#include <vector>

enum EHttpMethods { GET, POST, DELETE, NONE };

enum HttpVersion { HTTP_1_1, HTTP_2_0, HTTP_3_0 };

std::vector<std::string> splitHeader(std::string headerString,
                                     bool withNewline);

/* All env variables for execve */
extern char **environ;

/**
 * Utils
 */
std::string unChunk(std::string body);

#endif