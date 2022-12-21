#include <cstdio>

#include "HttpRequest.hpp"

DeleteRequest::DeleteRequest(HttpHeaderData const &data) : HttpRequest(data) {}

DeleteRequest::DeleteRequest(const DeleteRequest &ref) : HttpRequest(ref) {}

DeleteRequest::~DeleteRequest() {}

HTTPStatusCode DeleteRequest::executeRequest(const Server &server) {
  // int rem = remove(_url.c_str()); // need to add the root as prefix to uri to
  // find the right local file if (rem != 0)
  //   return 403;
  (void)server;
  return HTTPStatusCode::OK;
}

HttpResponse DeleteRequest::constructResponse(const Server &server) {
  HTTPStatusCode responseStatus = executeRequest(server);
  (void)responseStatus;
  return HttpResponse();
}
