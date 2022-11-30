#include "HttpRequest.hpp"

PostRequest::PostRequest(HttpHeaderData const &data) : HttpRequest(data), _body(data.body) {}

PostRequest::PostRequest(const PostRequest &ref) : HttpRequest(ref) {}

PostRequest::~PostRequest() {}

int PostRequest::executeRequest(Server &server)
{
  if (!isMethodAllowed(server, _uri, POST))
    return 405;
  // Post the body to a file (std::string _body)
  return 0;
}

HttpResponse PostRequest::constructResponse(Server &server, std::string &index)
{
  int responseStatus = executeRequest(server);
  (void)index;
  if (!responseStatus)
    return HttpResponse(HTTP_1_1, 204, "OK");
  return HttpResponse(HTTP_1_1, responseStatus, _parseResponseStatus(responseStatus));
}
