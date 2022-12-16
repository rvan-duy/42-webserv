#include <HttpRequest.hpp>
#include "Logger.hpp"

/*
 *  Canonical
 */
HttpRequest::HttpRequest(HttpHeaderData const &data) : HttpMessage(data.headers, data.httpVersion, data.body), _method(data.method), _url(data.url), _chunked(data.chunked) {}

HttpRequest::HttpRequest() {}

HttpRequest::HttpRequest(const HttpRequest &obj) : HttpMessage(obj)
{
  _method = obj._method;
  _url = obj._url;
  _chunked = obj._chunked;
}

HttpRequest::~HttpRequest() {}

/*
 * Getters
 */
EHttpMethods HttpRequest::getMethod() const
{
  return _method;
}

std::string HttpRequest::getUrl() const
{
  return _url;
}

bool HttpRequest::getChunked() const
{
  return _chunked;
}

// **
// still can have conflicts with /file location and uri being /filename this will be looked in as file even though
// it should be under / only..
// coudl maybe fix by adding a / after location routes when it is a directory.. ?
bool HttpRequest::isMethodAllowed(Server &server, std::string uri, EHttpMethods method)
{
  std::vector<Route> routes = server.getRoutes();
  unsigned long maxlen = 0;
  bool isAllowed = true;

  for (std::vector<Route>::iterator it = routes.begin(); it != routes.end(); it++)
  {
    if (0 == uri.find(it->route)) // **
    {
      if (maxlen < it->route.size())
      {
        isAllowed = it->allowedMethods.at(method);
        maxlen = it->route.size();
      }
    }
  }
  return isAllowed;
}
