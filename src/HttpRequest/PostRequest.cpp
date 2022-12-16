#include "HttpRequest.hpp"

PostRequest::PostRequest(std::string& msg) : HttpRequest(msg) {}

PostRequest::PostRequest(const PostRequest& ref) : HttpRequest(ref)  {}

PostRequest::~PostRequest() {}

int PostRequest::executeRequest(const Server& server) {
  // if (!isMethodAllowed(server, _uri, POST))
  //   return 405;
  // Post the body to a file (std::string _body)
  return 0;
}

HttpResponse PostRequest::constructResponse(const Server& server) {
  int responseStatus = executeRequest(server);
  (void)index;
  if (!responseStatus) {

    return HttpResponse();
  }
  return HttpResponse();
}
