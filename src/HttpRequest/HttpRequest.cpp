#include "HttpRequest.hpp"
#include "Logger.hpp"

/*
 *  Canonical
 */
HttpRequest::HttpRequest() {}

HttpRequest::HttpRequest(const std::string& msg) {
  extractInitialResponsLine(msg);
  extractHeaders(msg);
  extractBody(msg);
}

HttpRequest::HttpRequest(const HttpRequest &obj) : HttpMessage(obj) {
  _method = obj._method;
  _uri    = obj._uri;
}

HttpRequest::~HttpRequest() {}

/*
 *  Helper functions
 */
std::string  extractArgument(const std::string& msg, int n) {
  std::string tmp;
  size_t  start;

  start = 0;
  while (n)
  {
    while (msg[start] == ' ')
      start++;
    if (n == 1)
    {
      std::string tmpo = msg.substr(start, msg.find_first_of(std::string("\r\n "), start) - start);
      return(tmpo);
    }
    start = msg.find(' ', start);
    n--;
  }
  return tmp;
}

static void addHeader(std::map<std::string, std::string>& headers, const std::string& msg, size_t start) {
  std::string key;
  std::string value;

  size_t colon = msg.find(":", start);

  key = msg.substr(start, colon - start);
  value = msg.substr(colon + 2, msg.find("\r\n", start) - colon - 2);
  // std::cout << key << " : " << value << std::endl;
  headers[key] = value;
}

/*
 *  Extracts the method, uri & http-version from the first line of the request.
 */
void  HttpRequest::extractInitialResponsLine(const std::string& msg) {
  Logger &logger = Logger::getInstance();

  _method = _parseMethod(extractArgument(msg, 1));
  _uri = extractArgument(msg, 2);
  _version = _parseVersion(extractArgument(msg, 3));

  logger.log("HttpRequest:\n\t\tmethod:\t\t" + extractArgument(msg, 1) + "\n\t\tversion:\t" + extractArgument(msg, 3) + "\n\t\turi:\t\t" + _uri);
}

/*
 *  Extracts the headers from the top of the request.
 */
void  HttpRequest::extractHeaders(const std::string& msg) {
  Logger &logger = Logger::getInstance();
  size_t  i;

  i = 2 + msg.find("\r\n", 0, 2);
  while (msg.find("\r\n\r\n", i, 4) != std::string::npos)
  {
    addHeader(_headers, msg, i);
    i = 2 + msg.find("\r\n", i, 2);
  }

  logger.log("Headers have been added");
}

/*
 *  Extracts the body part of the string.
 */
void  HttpRequest::extractBody(const std::string& msg) {
  Logger &logger = Logger::getInstance();

  _body = msg.substr(4 + msg.find("\r\n\r\n", 0, 4));
  std::cout << _body << std::endl;

  logger.log("body parsed:\t" + (std::string)(_body.empty() ? "true" : "false"));
}

/*
 * Getter for the request method
 */
EHttpMethods HttpRequest::getMethod() const {
  return _method;
}

/*
 * Getter for the request URI
 */
std::string HttpRequest::getUri() const {
  return _uri;
}

bool  HttpRequest::getChunked() const {
  return _chunked;
}

/*
 * Parse the method string into the method enum
 * @param method the method string
 * @return the method enum
 */
EHttpMethods _parseMethod(const std::string &method) {
  Logger &logger = Logger::getInstance();
  std::map<std::string, EHttpMethods> method_map;

  method_map["GET"]    = GET;
  method_map["POST"]   = POST;
  method_map["DELETE"] = DELETE;

  try {
    return method_map.at(method);
  } catch (std::exception& e) {
    // _status_code = 400;
    logger.error("bad request: method unsupported ( " + method + " )");
    return NONE;
  }
}
