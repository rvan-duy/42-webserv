#pragma once

#include <string>
#include <vector>

enum EHttpMethods { GET, POST, DELETE, NONE };

enum HttpVersion { HTTP_1_1, HTTP_2_0, HTTP_3_0 };

std::vector<std::string> splitHeader(std::string headerString,
                                     bool withNewline);
