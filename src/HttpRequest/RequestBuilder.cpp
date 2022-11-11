#include "HttpRequest.hpp"
#include "Server.hpp"

//  MEMBERS
HttpRequest *Server::createRequest(std::string& msg)
{
  switch (_parseMethod(extractArgument(msg, 1))) 
  {
    case GET:
        return new GetRequest(target);
    case POST:
        return new PostRequest(target);
    case DELETE:
        return new DeleteRequest(target);
    default:
        return new HttpRequest(target);
  }
  return NULL;
}

void  Server::buildRequest(std::string& msg, int fd) {
  _requests[fd] = createRequest(msg);
}
