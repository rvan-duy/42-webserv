#include <regex>
#include <string>

bool isValidIpAdress(std::string const rawLine)
{
    const std::regex ipAdressRegex("^(?:[0-9]{1,3}\\.){3}[0-9]{1,3}$");

    if (rawLine.length() > 15 || !std::regex_match(rawLine, ipAdressRegex))
        return false;
    return true;
}