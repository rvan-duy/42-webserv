#include "HttpRequest.hpp"

#include <cstdio>

DeleteRequest::DeleteRequest(std::string& msg) : HttpRequest(msg) {}

DeleteRequest::DeleteRequest(const DeleteRequest& ref) : HttpRequest(ref)  {}

DeleteRequest::~DeleteRequest() {}

int DeleteRequest::executeRequest(const Server& server) {
  // if (!isMethodAllowed(server, _uri, DELETE))
  //    return 405;
  int rem = remove(_uri.c_str()); // need to add the root as prefix to uri to find the right local file
  if (rem != 0)
    return 403;
  return rem;
}

HttpResponse DeleteRequest::constructResponse(const Server& server) {
  int responseStatus = executeRequest(server);
  (void)index;
  return HttpResponse();
}
