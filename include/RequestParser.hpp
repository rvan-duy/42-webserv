#pragma once

#include <HttpRequest.hpp>
#include <string>

#define MAX_HEADER_SIZE 8000

class RequestParser;

enum EContentType
{
    FORMDATA,
};

/* Parser member function typedef */
typedef int (RequestParser::*RequestParserFunc)(void *dest, std::vector<std::string> dataLine);

/* Key - parser function pair */
typedef struct s_requestParseFuncPair
{
    std::string key;
    RequestParserFunc func;
} t_requestParseFuncPair;

struct HttpHeaderData
{
    HttpHeaderData() : contentLength(0) {}
    HttpVersion httpVersion;
    std::string url;
    EHttpMethods method;
    std::string host;
    EContentType contentType;
    std::string boundary;
    size_t contentLength;
    std::string body;
};

class RequestParser
{
public:
    static HttpRequest *parseHeader(std::string &rawRequest);
};