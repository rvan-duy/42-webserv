#include <HttpRequest.hpp>
#include "Logger.hpp"

/*
 *  Canonical
 */
HttpRequest::HttpRequest(HttpHeaderData const &data) : HttpMessage(data.headers, data.httpVersion, data.body), _method(data.method), _uri(data.url), _chunked(data.chunked) {}

HttpRequest::HttpRequest() {}

HttpRequest::HttpRequest(const HttpRequest &obj) : HttpMessage(obj)
{
  _method = obj._method;
  _uri = obj._uri;
}

HttpRequest::~HttpRequest() {}

/*
 * Getters
 */
EHttpMethods HttpRequest::getMethod() const
{
  return _method;
}

std::string HttpRequest::getUri() const
{
  return _uri;
}

bool HttpRequest::getChunked() const
{
  return _chunked;
}

/*
 *  Extract a certain argument from the HttpMessage
 */
std::string extractArgument(const std::string &msg, int n)
{
  std::string tmp;
  size_t start;

  start = 0;
  while (n)
  {
    while (msg[start] == ' ')
      start++;
    if (n == 1)
    {
      std::string tmpo = msg.substr(start, msg.find_first_of(std::string("\r\n "), start) - start);
      return (tmpo);
    }
    start = msg.find(' ', start);
    n--;
  }
  return tmp;
}

/*
 * Parse the method string into the method enum
 * @param method the method string
 * @return the method enum
 */
EHttpMethods _parseMethod(const std::string &method)
{
  Logger &logger = Logger::getInstance();
  std::map<std::string, EHttpMethods> method_map;

  method_map["GET"] = GET;
  method_map["POST"] = POST;
  method_map["DELETE"] = DELETE;

  try
  {
    return method_map.at(method);
  }
  catch (std::exception &e)
  {
    // _status_code = 400;
    logger.error("bad request: method unsupported ( " + method + " )");
    return NONE;
  }
}

/* This needs to include all the response statuses used in executeRequest */
std::string _parseResponseStatus(const int &status)
{
  std::map<int, std::string> response_map;

  response_map[403] = "Forbidden";
  response_map[404] = "Not found";
  response_map[405] = "Method not alowed";
  response_map[409] = "Conflict";

  return response_map.at(status);
}

// **
// still can have conflicts with /file location and uri being /filename this will be looked in as file even though
// it should be under / only..
// coudl maybe fix by adding a / after location routes when it is a directory.. ?
bool isMethodAllowed(Server &server, std::string uri, EHttpMethods method)
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
