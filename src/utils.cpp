#include <regex>
#include <StatusCodes.hpp>

std::string getMessageByStatusCode(HTTPStatusCode statusCode)
{
    switch (statusCode)
    {
    case HTTPStatusCode::OK:
        return "OK";
    case HTTPStatusCode::INTERNAL_SERVER_ERROR:
        return "Internal server error";
    case HTTPStatusCode::METHOD_NOT_ALLOWED:
        return "Method not allowed";
    case HTTPStatusCode::CONTENT_TOO_LARGE:
        return "Content too lage";
    case HTTPStatusCode::I_AM_A_TEAPOT:
        return "I am a teapot";
    default:
        return "Bad request";
    }
}
