#include <cstdio>
#include <HttpRequest.hpp>

BadRequest::BadRequest(HTTPStatusCode statusCode) : HttpRequest(), _statusCode(statusCode) {}

BadRequest::BadRequest(const BadRequest &ref) : HttpRequest(ref) {}

BadRequest::~BadRequest() {}

HTTPStatusCode BadRequest::executeRequest(Server &server)
{
    (void)server;
    return HTTPStatusCode::OK;
}

HttpResponse BadRequest::constructResponse(Server &server, std::string &index)
{
    (void)server;
    (void)index;

    std::string message = getMessageByStatusCode(_statusCode);
    return HttpResponse(HTTP_1_1, _statusCode, message);
}