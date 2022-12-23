#include "HttpRequest.hpp"

PostRequest::PostRequest(HttpHeaderData const &data) : HttpRequest(data) {}

PostRequest::PostRequest(const PostRequest &ref) : HttpRequest(ref) {}

PostRequest::~PostRequest() {}
