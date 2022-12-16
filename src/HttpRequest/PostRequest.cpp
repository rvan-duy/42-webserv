#include "HttpRequest.hpp"

PostRequest::PostRequest(HttpHeaderData const &data) : HttpRequest(data) {}

PostRequest::PostRequest(const PostRequest &ref) : HttpRequest(ref) {}

PostRequest::~PostRequest() {}

HTTPStatusCode PostRequest::executeRequest(Server &server)
{
  if (!isMethodAllowed(server, _url, POST))
    return HTTPStatusCode::METHOD_NOT_ALLOWED;
  // Post the body to a file (std::string _body)
  return HTTPStatusCode::OK;
}

HttpResponse PostRequest::constructResponse(Server &server, std::string &index)
{
  HTTPStatusCode responseStatus = executeRequest(server);
  (void)index;
  if (responseStatus == HTTPStatusCode::OK)
    return HttpResponse(HTTP_1_1, HTTPStatusCode::OK, "OK");
  return HttpResponse(HTTP_1_1, responseStatus, getMessageByStatusCode(responseStatus));
}
