#include "HttpRequest.hpp"

#include <cstdio>

BadRequest::BadRequest(std::string& msg) : HttpRequest(msg) {}

BadRequest::BadRequest(const BadRequest& ref) : HttpRequest(ref)  {}

BadRequest::~BadRequest() {}

int BadRequest::executeRequest(const Server& server) {
  (void)server;
  return (0);
}

HttpResponse BadRequest::constructResponse(const Server& server) {
  (void)server;
  (void)index;
  return HttpResponse();
}