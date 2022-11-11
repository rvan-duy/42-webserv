#include "HttpRequest.hpp"
#include "Server.hpp"

//  MEMBERS
HttpRequest *Server::createRequest(std::string& msg)
{
  switch (_parseMethod(extractArgument(msg, 1))) 
  {
    case GET:
        return new GetRequest(msg);
    case POST:
        return new PostRequest(msg);
    case DELETE:
        return new DeleteRequest(msg);
    default:
        return NULL;
  }
  return NULL;
}

void  Server::buildRequest(std::string& msg, int fd) {
  _requests[fd] = createRequest(msg);
}
