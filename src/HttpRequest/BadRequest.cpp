#include <HttpRequest.hpp>
#include <cstdio>

BadRequest::BadRequest(HTTPStatusCode statusCode) : HttpRequest() {
      _statusCode = statusCode;
}

BadRequest::BadRequest(const BadRequest &ref) : HttpRequest(ref) {}

BadRequest::~BadRequest() {}

HTTPStatusCode BadRequest::executeRequest(const Server &server) {
  (void)server;
  return HTTPStatusCode::OK;
}

HttpResponse BadRequest::constructResponse(const Server &server) {
  (void)server;
  // std::string message = getMessageByStatusCode(_statusCode);
  // return HttpResponse(HTTP_1_1, _statusCode, message);
  (void)_statusCode;
  return HttpResponse();
}
