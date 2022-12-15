#include "HttpRequest.hpp"

#include <cstdio>

DeleteRequest::DeleteRequest(HttpHeaderData const &data) : HttpRequest(data) {}

DeleteRequest::DeleteRequest(const DeleteRequest &ref) : HttpRequest(ref) {}

DeleteRequest::~DeleteRequest() {}

int DeleteRequest::executeRequest(Server &server)
{
  if (!isMethodAllowed(server, _url, DELETE))
    return 405;
  int rem = remove(_url.c_str()); // need to add the root as prefix to uri to find the right local file
  if (rem != 0)
    return 403;
  return rem;
}

HttpResponse DeleteRequest::constructResponse(Server &server, std::string &index)
{
  int responseStatus = executeRequest(server);
  (void)index;
  if (!responseStatus)
    return HttpResponse(HTTP_1_1, 204, "OK");
  return HttpResponse(HTTP_1_1, responseStatus, _parseResponseStatus(responseStatus));
}
