#include <Parser.hpp>

/**************************************************/
/* Parsing abstract syntax tree lines	          */
/**************************************************/

t_parseFuncPair Parser::lineParsingFuncs[PARSER_FUNC_N] = {
    {"listen", &Parser::parsePort},
    {"serverName", &Parser::parseServerName},
    {"maxBodySize", &Parser::parseMaxBodySize},
    {"return", &Parser::parseHost},
};

int Parser::parseServerName(void *dest, t_dataLine line)
{
    if (!dest || line.size() < 2)
    {
        return 1;
    }
    Server *server = static_cast<Server *>(dest);
    std::vector<std::string> serverName;
    std::vector<std::string>::iterator it = line.begin();
    it++;
    while (it != line.end())
    {
        serverName.push_back(*it);
        it++;
    }
    server->setServerName(serverName);
    return 0;
}

int Parser::parseHost(void *dest, t_dataLine line)
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
    return server->setHost(std::stoi(statusCode), line.at(2));
}

int Parser::parsePort(void *dest, t_dataLine line)
{
    if (!dest || line.size() != 2)
    {
        return 1;
    }
    Server *server = static_cast<Server *>(dest);
    std::string port;

    port = line.at(1);
    for (size_t i = 0; i < port.length(); i++)
    {
        if (!isdigit(port[i]))
        {
            return 1;
        }
    }
    return server->setPort(std::stoi(line.at(1)));
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
