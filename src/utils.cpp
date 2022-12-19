#include <regex>
#include <StatusCodes.hpp>

bool isValidIpAdress(std::string const rawLine)
{
    const std::regex ipAdressRegex("^(?:[0-9]{1,3}\\.){3}[0-9]{1,3}$");

    if (rawLine.length() > 15 || !std::regex_match(rawLine, ipAdressRegex))
        return false;
    return true;
}

std::string getMessageByStatusCode(HTTPStatusCode statusCode)
{
    switch (statusCode)
    {
    case HTTPStatusCode::OK:
        return "OK";
    case HTTPStatusCode::BAD_REQUEST:
        return "Bad request";
    case HTTPStatusCode::NOT_FOUND:
        return "Not found";
    case HTTPStatusCode::METHOD_NOT_ALLOWED:
        return "Method not allowed";
    case HTTPStatusCode::NOT_ACCEPTABLE:
        return "Not acceptable";
    case HTTPStatusCode::CONTENT_TOO_LARGE:
        return "Content too large";
    case HTTPStatusCode::I_AM_A_TEAPOT:
        return "I am a teapot";
    case HTTPStatusCode::INTERNAL_SERVER_ERROR:
        return "Internal server error";
    default:
        return "Bad request";
    }
}
