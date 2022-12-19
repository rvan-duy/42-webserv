#include "HttpRequest.hpp"

PostRequest::PostRequest(HttpHeaderData const &data) : HttpRequest(data) {}

PostRequest::PostRequest(const PostRequest &ref) : HttpRequest(ref) {}

PostRequest::~PostRequest() {}

HTTPStatusCode PostRequest::executeRequest(const Server &server)
{
  (void)server;
  return HTTPStatusCode::OK;
}

HttpResponse PostRequest::constructResponse(const Server& server) {
  HTTPStatusCode responseStatus = executeRequest(server);
  return HttpResponse();
}
