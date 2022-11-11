#include "HttpRequest.hpp"

PostRequest::PostRequest(std::string& msg) : HttpRequest(msg) {}

PostRequest::PostRequest(const PostRequest& ref) : HttpRequest(ref)  {}

PostRequest::~PostRequest() {}

void PostRequest::executeRequest() {
  return ;
}

HttpResponse PostRequest::constructResponse(Server& server, std::string& index) {
  return HttpResponse();
}
