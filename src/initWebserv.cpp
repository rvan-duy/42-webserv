#include <algorithm>
#include <iostream>
#include <Server.hpp>
#include <Socket.hpp>
#include <Lexer.hpp>
#include <Parser.hpp>
#include <Logger.hpp>

static int openFile(std::ifstream &cFile, std::string const &filePath)
{
    Logger &logger = Logger::getInstance();
    cFile.open(filePath);
    if (!cFile.is_open())
        return 1;
    logger.log("Opened config file");
    return 0;
}

static int parseFile(std::vector<Server> *pServers, std::ifstream &cFile)
{
    std::vector<Token> tokens = Lexer::tokenizeFile(cFile);
    Parser parser(tokens);
    if (parser.parseTokens(pServers))
        return 1;
    return 0;
}

/**
 * Per unique port in servers a socket must be opened
 */
static void setupSockets(std::vector<Socket> *pSockets, std::vector<Server> &servers)
{
    std::vector<int> ports;
    for (std::vector<Server>::iterator it = servers.begin(); it != servers.end(); ++it)
    {
        if (std::find(ports.begin(), ports.end(), it->getPort()) == std::end(ports))
            ports.push_back(it->getPort());
    }
    for (std::vector<int>::iterator portsIt = ports.begin(); portsIt != ports.end(); ++portsIt)
    {
        pSockets->push_back(Socket(*portsIt));
    }
}

/**
 * Every server that listens to same port as socket is added to the list
 */
static void matchSocketsAndServers(std::vector<Socket> *pSockets, std::vector<Server> servers)
{
    for (std::vector<Socket>::iterator socketIt = pSockets->begin(); socketIt != pSockets->end(); ++socketIt)
    {
        for (std::vector<Server>::iterator serverIt = servers.begin(); serverIt != servers.end(); ++serverIt)
        {
            if (socketIt->getPort() == serverIt->getPort())
                socketIt->addServer(*serverIt);
        }
    }
}

void prepareSockets(std::vector<Socket> &socks)
{
    for (std::vector<Socket>::iterator it = socks.begin(); it != socks.end(); it++)
    {
        it->prepare();
    }
}

int initWebserver(std::vector<Socket> *pSockets, std::string const &filePath)
{
    Logger &logger = Logger::getInstance();
    std::vector<Server> servers;
    std::ifstream cFile;

    if (openFile(cFile, filePath))
    {
        logger.error("Error opening file: " + std::string(strerror(errno)));
        return 1;
    }
    if (parseFile(&servers, cFile))
    {
        logger.error("Error parsing tokens from configfile");
        return 1;
    }
    logger.log("Config file successfully parsed");
    cFile.close();
    setupSockets(pSockets, servers);
    prepareSockets(*pSockets);
    matchSocketsAndServers(pSockets, servers);
    return 0;
}
