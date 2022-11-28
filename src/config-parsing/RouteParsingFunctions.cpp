#include <Parser.hpp>

/**************************************************/
/* Parsing abstract syntax tree blocks	          */
/**************************************************/

t_parseFuncPair Parser::blockParsingFuncs[BLOCK_FUNC_N] = {
    {"root", &Parser::parseRoot},
    {"index", &Parser::parsePort},
    {"autoIndex", &Parser::parsePort},
    {"cgi", &Parser::parseCgi},
    {"methods", &Parser::parseMethods},
};

int Parser::parseMethods(void *dest, t_dataLine line)
{
    if (!dest || line.size() < 2 || line.size() > 4)
    {
        return 1;
    }
    Route *route = static_cast<Route *>(dest);
    route->allowedMethods[GET] = false;
    route->allowedMethods[POST] = false;
    route->allowedMethods[DELETE] = false;
    for (size_t i = 1; i < line.size(); i++)
    {
        if (line.at(i) == "GET")
        {
            if (route->allowedMethods[GET] == true)
            {
                return 1;
            }
            route->allowedMethods[GET] = true;
        }
        else if (line.at(i) == "POST")
        {
            if (route->allowedMethods[POST] == true)
            {
                return 1;
            }
            route->allowedMethods[POST] = true;
        }
        else if (line.at(i) == "DELETE")
        {
            if (route->allowedMethods[DELETE] == true)
            {
                return 1;
            }
            route->allowedMethods[DELETE] = true;
        }
        else
        {
            Logger::getInstance().error("[CONFIG PARSER]: Invalid method set");
            return 1;
        }
    }
    return 0;
}

int Parser::parseRoot(void *dest, t_dataLine line)
{
    if (!dest || line.size() != 2)
    {
        return 1;
    }
    Route *route = static_cast<Route *>(dest);
    if (route->rootDirectory != "")
    {
        Logger::getInstance().error("[CONFIG PARSER]: Root directory for route already set");
        return 1;
    }
    route->rootDirectory = line.at(1);
    return 0;
}

int Parser::parseIndex(void *dest, t_dataLine line)
{
    if (!dest || line.size() != 2)
    {
        return 1;
    }
    // Route *route = static_cast<Route *>(dest);
    // TODO
    return 0;
}

int Parser::parseAutoIndex(void *dest, t_dataLine line)
{
    if (!dest || line.size() != 2)
    {
        return 1;
    }
    Route *route = static_cast<Route *>(dest);
    // TODO: can this error?
    if (line.at(2) == "on")
    {
        route->autoIndex = true;
    }
    else if (line.at(2) == "off")
    {
        route->autoIndex = false;
    }
    else
    {
        // TODO
    }
    return 0;
}

// TODO: implement php
int Parser::parseCgi(void *dest, t_dataLine line)
{
    if (!dest || line.size() != 3)
    {
        Logger::getInstance().error("Cgi route invalid");
        return 1;
    }
    Route *route = static_cast<Route *>(dest);
    if (route->cgiRoot.length() != 0)
    {
        Logger::getInstance().error("CGI root already defined");
        return 1;
    }
    std::string extension = line.at(2);
    if (extension == ".py")
    {
        // TODO: save extension
    }
    else
    {
        Logger::getInstance().error("Extension invalid");
        return 1;
    }
    route->cgiRoot = line.at(3);
    return 0;
}
