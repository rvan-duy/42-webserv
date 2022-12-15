#include <Parser.hpp>

/**************************************************/
/* Parsing abstract syntax tree lines	          */
/**************************************************/

t_parseFuncPair Parser::lineParsingFuncs[PARSER_FUNC_N] = {
    {"listen", &Parser::parsePortAndIp},
    {"serverName", &Parser::parseServerName},
    {"errorPage", &Parser::parseErrorPage},
    {"maxBodySize", &Parser::parseMaxBodySize},
};

int Parser::parseServerName(void *dest, t_dataLine line)
{
    if (!dest || line.size() < 2)
    {
        return 1;
    }
    Server *server = static_cast<Server *>(dest);
    std::string serverName = "";
    std::vector<std::string>::iterator it = line.begin();
    it++;
    while (it != line.end())
    {
        if (serverName.length() != 0)
        {
            serverName += " ";
        }
        serverName += *it;
        it++;
    }
    server->setServerName(serverName);
    return 0;
}

int Parser::parseErrorPage(void *dest, t_dataLine line)
{
    if (!dest || line.size() != 3)
    {
        return 1;
    }
    Server *server = static_cast<Server *>(dest);
    std::string statusCode;

    statusCode = line.at(1);
    for (size_t i = 0; i < statusCode.length(); i++)
    {
        if (!isdigit(statusCode[i]))
        {
            return 1;
        }
    }
    return server->setErrorPage(std::stoi(statusCode), line.at(2));
}

static int parsePort(std::string const &rawLine)
{
    size_t port;
    for (size_t i = 0; i < rawLine.length(); i++)
    {
        if (!isdigit(rawLine[i]))
        {
            Logger::getInstance().error("Invalid port entered");
            return -1;
        }
    }
    port = std::stoi(rawLine);
    if (port > MAX_PORT)
    {
        Logger::getInstance().error("Invalid port entered");
        return -1;
    }
    return port;
}

static bool isAllNumbers(std::string const &line)
{
    for (size_t i = 0; i < line.length(); i++)
    {
        if (!isdigit(line[i]))
        {
            return false;
        }
    }
    return true;
}

int Parser::parsePortAndIp(void *dest, t_dataLine line)
{
    if (!dest || line.size() != 2)
    {
        return 1;
    }
    Server *server = static_cast<Server *>(dest);
    std::string rawLine = line.at(1);
    size_t port = 0;
    /* In case only port is entered */
    if (isAllNumbers(rawLine))
    {
        port = std::stoi(rawLine);
        if (port > INT_MAX)
        {
            Logger::getInstance().error("Invalid port entered");
            return 1;
        }
        return server->setPort(port);
    }
    /* Check if port AND ip adress are entered */
    size_t semiCol = rawLine.find(':');
    if (semiCol != std::string::npos)
    {
        /* Parse port, then cut it out from string */
        port = parsePort(rawLine.substr(semiCol + 1, rawLine.length() - semiCol + 1));
        if (port < 0)
        {
            Logger::getInstance().error("Invalid port entered");
            return 1;
        }
        rawLine = rawLine.substr(0, semiCol);
    }
    if (!isValidIpAdress(rawLine))
    {
        Logger::getInstance().error("Invalid ip adress entered");
        return 1;
    }
    if (port != 0 && server->setPort(port))
        return 1;
    return server->setHost(rawLine);
}

int Parser::parseMaxBodySize(void *dest, t_dataLine line)
{
    if (!dest || line.size() != 2)
    {
        return 1;
    }
    Server *server = static_cast<Server *>(dest);
    std::string maxBodySize;

    maxBodySize = line.at(1);
    for (size_t i = 0; i < maxBodySize.length(); i++)
    {
        if (!isdigit(maxBodySize[i]))
        {
            return 1;
        }
    }
    return server->setMaxBody(std::stoi(line.at(1)));
}
