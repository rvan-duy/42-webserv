#include <HttpRequest.hpp>

#include "Logger.hpp"

/*
 *  Canonical
 */
HttpRequest::HttpRequest(HttpHeaderData const &data)
    : HttpMessage(data.httpVersion, data.headers, data.body),
      _method(data.method),
      _uri(data.url) {}

HttpRequest::HttpRequest() {}

HttpRequest::HttpRequest(const HttpRequest &obj) : HttpMessage(obj) {
  _method = obj._method;
  _uri = obj._uri;
}

HttpRequest::~HttpRequest() {}

/*
 * Protected methods
 */

bool HttpRequest::_isMethodAllowed(
    const std::map<EHttpMethods, bool> allowedMethods) const {
  EHttpMethods currentMethod = _method;

  for (std::map<EHttpMethods, bool>::const_iterator it = allowedMethods.begin();
       it != allowedMethods.end(); it++) {
    if (it->first == currentMethod) {
      return it->second;
    }
  }
  return false;
}

HttpRequest *HttpRequest::operator+(const HttpRequest& chunk) {
  if (chunk._statusCode != HTTPStatusCode::NOT_SET)
  {
    delete this;
    return new BadRequest(HTTPStatusCode::BAD_REQUEST);
  }
  _body += chunk._body;
  _headers.insert(std::begin(chunk._headers), std::end(chunk._headers));
  return this;
}

void  HttpRequest::unChunkBody() {
  _body = unChunk(_body);
}

/**
 * Determines whether a given HTTP request uses chunked transfer encoding.
 */
bool  HttpRequest::isFirstChunk() {
  return "chunked" == getHeader("Transfer-Encoding");
}
