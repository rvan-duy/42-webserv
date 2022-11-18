#include <Webserver.hpp>

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

int initWebserver(std::vector<Server> *pServers, std::string const &filePath)
{
    Logger &logger = Logger::getInstance();
    std::ifstream cFile;

    if (openFile(cFile, filePath))
    {
        logger.error("Error opening file: " + std::string(strerror(errno)));
        return 1;
    }
    if (parseFile(pServers, cFile))
    {
        logger.error("Error parsing tokens from configfile");
        return 1;
    }
    logger.log("Tokens successfully parsed");
    cFile.close();
    return 0;
}
