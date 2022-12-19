#include <HttpRequest.hpp>
#include "Logger.hpp"

/*
 *  Canonical
 */
HttpRequest::HttpRequest(HttpHeaderData const &data) : HttpMessage(data.headers, data.httpVersion, data.body), _method(data.method), _uri(data.url), _chunked(data.chunked) {}

HttpRequest::HttpRequest() : _chunked(false) {}

HttpRequest::HttpRequest(const HttpRequest &obj) : HttpMessage(obj)
{
  _method = obj._method;
  _uri = obj._uri;
  _chunked = obj._chunked;
}

HttpRequest::~HttpRequest() {}

HttpRequest *HttpRequest::operator+(const HttpRequest& chunk) {
  _body += chunk._body;
  _headers.insert(std::begin(chunk._headers), std::end(chunk._headers));
  return this;
}

/*
 * Protected methods
 */

bool HttpRequest::_isMethodAllowed(const std::map<EHttpMethods, bool> allowedMethods) const
{
  EHttpMethods currentMethod = _method;

  for (std::map<EHttpMethods, bool>::const_iterator it = allowedMethods.begin(); it != allowedMethods.end(); it++)
  {
    if (it->first == currentMethod)
    {
      return it->second;
    }
  }
  return false;
}
