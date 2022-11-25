#include "HttpRequest.hpp"
#include "Server.hpp"

//  MEMBERS
HttpRequest *Server::createRequest(std::string& msg)
{
  Logger& logger = Logger::getInstance();
  switch (_parseMethod(extractArgument(msg, 1))) 
  {
    case GET:
        return new GetRequest(msg);
    case POST:
        return new PostRequest(msg);
    case DELETE:
        return new DeleteRequest(msg);
    default:
        logger.debug("failed to parse request: \n" + msg);
        return NULL;
  }
  return NULL;
}

void  Server::buildRequest(std::string& msg, int fd) {
  Logger &logger = Logger::getInstance();
  _requests[fd] = createRequest(msg);
  logger.info("created request");
}
