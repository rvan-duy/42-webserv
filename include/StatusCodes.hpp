#pragma once

#include <string>

enum class HTTPStatusCode
{
    OK = 200,
    BAD_REQUEST = 400,
    NOT_FOUND = 404,
    METHOD_NOT_ALLOWED = 405,
    NOT_ACCEPTABLE = 406,
    CONTENT_TOO_LARGE = 413,
    I_AM_A_TEAPOT = 418,
    INTERNAL_SERVER_ERROR = 500,
};

std::string getMessageByStatusCode(HTTPStatusCode statusCode);
