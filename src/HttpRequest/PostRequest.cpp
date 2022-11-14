#include "HttpRequest.hpp"

PostRequest::PostRequest(std::string& msg) : HttpRequest(msg) {}

PostRequest::PostRequest(const PostRequest& ref) : HttpRequest(ref)  {}

PostRequest::~PostRequest() {}

int PostRequest::executeRequest() {
  if (!isMethodAllowed(server, _uri, POST))
    return 405;
  // Post the body to a file
  return 0;
}

HttpResponse PostRequest::constructResponse(Server& server, std::string& index) {
  int responseStatus = executeRequest();
  if (!responseStatus)
    return HttpResponse(HTTP_1_1, 204, "OK");
  return HttpResponse(HTTP_1_1, responseStatus, _parseResponseStatus(responseStatus));
}
