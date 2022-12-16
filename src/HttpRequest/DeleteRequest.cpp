#include "HttpRequest.hpp"

#include <cstdio>

DeleteRequest::DeleteRequest(HttpHeaderData const &data) : HttpRequest(data) {}

DeleteRequest::DeleteRequest(const DeleteRequest &ref) : HttpRequest(ref) {}

DeleteRequest::~DeleteRequest() {}

HTTPStatusCode DeleteRequest::executeRequest(Server &server)
{
  if (!isMethodAllowed(server, _url, DELETE))
    return HTTPStatusCode::METHOD_NOT_ALLOWED;
  // int rem = remove(_url.c_str()); // need to add the root as prefix to uri to find the right local file
  // if (rem != 0)
  //   return 403;
  return HTTPStatusCode::OK;
}

HttpResponse DeleteRequest::constructResponse(Server &server, std::string &index)
{
  HTTPStatusCode responseStatus = executeRequest(server);
  (void)index;
  if (responseStatus == HTTPStatusCode::OK)
    return HttpResponse(HTTP_1_1, HTTPStatusCode::OK, "OK");
  return HttpResponse(HTTP_1_1, responseStatus, getMessageByStatusCode(responseStatus));
}
