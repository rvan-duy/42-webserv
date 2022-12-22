#pragma once

#include <string>

enum EHttpMethods
{
    GET,
    POST,
    DELETE,
    NONE
};

enum HttpVersion
{
    HTTP_1_1,
    HTTP_2_0,
    HTTP_3_0
};

/**
 * Utils
 */
bool isValidIpAdress(std::string const rawLine);
std::string unChunk(std::string body);