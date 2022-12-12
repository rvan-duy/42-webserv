// #include "HttpRequest.hpp"

// #include <cstdio>

// BadRequest::BadRequest(std::string& msg) : HttpRequest(msg) {}

// BadRequest::BadRequest(const BadRequest& ref) : HttpRequest(ref)  {}

// BadRequest::~BadRequest() {}

// int BadRequest::executeRequest(Server& server) {
//   (void)server;
//   return (0);
// }

// HttpResponse BadRequest::constructResponse(Server& server, std::string& index) {
//   (void)server;
//   (void)index;
//   return HttpResponse(HTTP_1_1, 406, "Bad Request");
// }