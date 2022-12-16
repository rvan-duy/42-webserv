#include <cstdio>
#include <HttpRequest.hpp>

BadRequest::BadRequest(HttpHeaderData const &data) : HttpRequest(data) {}

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
    HttpResponse response;
    response._setResponse("root/404/index.html", 404, "Not Found", getVersion());
    // return HttpResponse(HTTP_1_1, 406, "Bad Request");
    return (response);
}