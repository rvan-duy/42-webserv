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
    std::string message = getErrorMessage();
    return HttpResponse(HTTP_1_1, _statusCode, message);
}

std::string BadRequest::getErrorMessage() const
{
    switch (_statusCode)
    {
    case INTERNAL_SERVER_ERROR:
        return "Internal server error";
    case METHOD_NOT_ALLOWED:
        return "Method not allowed";
    case CONTENT_TOO_LARGE:
        return "Content too lage";
    default:
        return "Bad request";
    }
}