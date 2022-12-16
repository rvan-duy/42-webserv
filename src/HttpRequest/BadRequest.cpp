#include <cstdio>
#include <HttpRequest.hpp>

BadRequest::BadRequest(int statusCode) : HttpRequest(), _statusCode(statusCode) {}

BadRequest::BadRequest(const BadRequest &ref) : HttpRequest(ref) {}

BadRequest::~BadRequest() {}

int BadRequest::executeRequest(Server &server)
{
    (void)server;
    return (0);
}

HttpResponse BadRequest::constructResponse(Server &server, std::string &index)
{
    (void)server;
    (void)index;
    std::string message = getMessageByStatusCode(_statusCode);
    return HttpResponse(HTTP_1_1, _statusCode, message);
}