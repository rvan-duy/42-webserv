#include <regex>
#include <StatusCodes.hpp>

bool isValidIpAdress(std::string const rawLine)
{
    const std::regex ipAdressRegex("^(?:[0-9]{1,3}\\.){3}[0-9]{1,3}$");

    if (rawLine.length() > 15 || !std::regex_match(rawLine, ipAdressRegex))
        return false;
    return true;
}

std::string getMessageByStatusCode(int const &statusCode)
{
    switch (statusCode)
    {
    case OK:
        return "OK";
    case INTERNAL_SERVER_ERROR:
        return "Internal server error";
    case METHOD_NOT_ALLOWED:
        return "Method not allowed";
    case CONTENT_TOO_LARGE:
        return "Content too lage";
    case I_AM_A_TEAPOT:
        return "I am a teapot";
    default:
        return "Bad request";
    }
}
