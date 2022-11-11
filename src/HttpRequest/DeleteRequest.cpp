#include "HttpRequest.hpp"

DeleteRequest::DeleteRequest(std::string& msg) : HttpRequest(msg) {}

DeleteRequest::DeleteRequest(const GetRequest& ref) : HttpRequest(ref)  {}

DeleteRequest::~DeleteRequest() {}

void DeleteRequest::executeRequest() {
  return ;
}

HttpResponse DeleteRequest::constructResponse(Server& server, std::string& index) {
  return HttpResponse();
}
