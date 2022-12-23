#include <cstdio>

#include "HttpRequest.hpp"

DeleteRequest::DeleteRequest(HttpHeaderData const &data) : HttpRequest(data) {}

DeleteRequest::DeleteRequest(const DeleteRequest &ref) : HttpRequest(ref) {}

DeleteRequest::~DeleteRequest() {}
