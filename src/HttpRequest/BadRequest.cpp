#include <HttpRequest.hpp>
#include <cstdio>

BadRequest::BadRequest(HTTPStatusCode statusCode)
    : HttpRequest(), _statusCode(statusCode) {}

BadRequest::BadRequest(const BadRequest &ref) : HttpRequest(ref) {}

BadRequest::~BadRequest() {}

HttpResponse BadRequest::executeRequest(const Server &server) {
  (void)server;
  Logger::getInstance().error(
      "[Bad request] -> sending response with statuscode: " +
          getMessageByStatusCode(_statusCode),
      VERBOSE);
  return HttpResponse(_statusCode);
}
