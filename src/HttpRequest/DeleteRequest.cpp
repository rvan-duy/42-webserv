#include <cstdio>

#include "HttpRequest.hpp"

DeleteRequest::DeleteRequest(HttpHeaderData const &data) : HttpRequest(data) {}

DeleteRequest::DeleteRequest(const DeleteRequest &ref) : HttpRequest(ref) {}

DeleteRequest::~DeleteRequest() {}

HttpResponse DeleteRequest::constructResponse(const Server &server) {
  return HttpResponse();
}
