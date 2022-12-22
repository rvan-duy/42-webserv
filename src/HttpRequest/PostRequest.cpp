#include "HttpRequest.hpp"

PostRequest::PostRequest(HttpHeaderData const &data) : HttpRequest(data) {}

PostRequest::PostRequest(const PostRequest &ref) : HttpRequest(ref) {}

PostRequest::~PostRequest() {}

HTTPStatusCode PostRequest::executeRequest(const Server &server) {
  (void)server;
  return HTTPStatusCode::OK;
}

HttpResponse PostRequest::constructResponse(const Server &server) {
  Route const &routeOfResponse = server.getRoute(HttpRequest::_uri);
  const std::string path = _constructPath(routeOfResponse.rootDirectory);

  if (_isMethodAllowed(routeOfResponse.allowedMethods) == false) {
    return HttpResponse(HTTPStatusCode::METHOD_NOT_ALLOWED);
  }

  if (_isCgiRequest(path)) {
    return _handleCgiRequest(path, routeOfResponse);
  }
  return _handleFileRequest(path, routeOfResponse);
}
