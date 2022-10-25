#include "HttpRequest.hpp"

#include "Logger.hpp"

HttpRequest::HttpRequest() {}

HttpRequest::HttpRequest(const HttpRequest &obj) : HttpMessage(obj) {
  _method = obj._method;
  _uri    = obj._uri;
}

HttpRequest::~HttpRequest() {}

/*
GET /hello.txt HTTP/1.1
User-Agent: curl/7.64.1
Host: www.example.com
Accept-Language: en, mi
*/

// Some notes:
// - The request string is a single string that contains the request line, headers, and body
// - The request line is case sensitive
// - There is no limit

/*
 * Parse the request string into the request object
 * @param message the request string
 */
void HttpRequest::parse(const std::string &message) {
  Logger &logger = Logger::getInstance();
  logger.log("Parsing request");

  /**************************************************/
  /* Start of by parsing the request line           */
  /**************************************************/

  logger.log("1. Parsing request line");
  std::string request_line = message.substr(0, message.find("\r\n"));
  std::string method       = request_line.substr(0, request_line.find(" "));
  std::string uri =
      request_line.substr(request_line.find(" ") + 1, request_line.rfind(" ") - request_line.find(" ") - 1);
  std::string version =
      request_line.substr(request_line.rfind(" ") + 1, request_line.length() - request_line.rfind(" ") - 1);
  request_line = method + " " + uri + " " + version;
  logger.log("- Request line parsed: [" + request_line + "]");

  /**************************************************/
  /* Parse the request method                       */
  /**************************************************/

  _method = _parse_method(method);
  logger.log("- Method parsed: [" + method + "]");

  /**************************************************/
  /* Parse the request URI                          */
  /**************************************************/

  _uri = uri;
  logger.log("- URI parsed: [" + uri + "]");

  /**************************************************/
  /* Parse the request version                      */
  /**************************************************/

  _version = _parse_version(version);
  logger.log("- Version parsed: [" + version + "]");

  /**************************************************/
  /* Parse the headers                              */
  /**************************************************/

  logger.log("2. Parsing headers:");
  std::string headers = message.substr(message.find("\r\n") + 2, message.rfind("\r\n") - message.find("\r\n") - 2);
  std::string header;
  while ((header = headers.substr(0, headers.find("\r\n"))) != "") {
    std::string key   = header.substr(0, header.find(":"));
    std::string value = header.substr(header.find(":") + 2, header.length() - header.find(":") - 2);
    _headers[key]     = value;
    headers           = headers.substr(headers.find("\r\n") + 2, headers.length() - headers.find("\r\n") - 2);
    logger.log("- [" + key + ": " + value + "]");
  }

  /**************************************************/
  /* Parse the body                                 */
  /**************************************************/

  logger.log("3. Parsing body");
  _body = message.substr(message.rfind("\r\n") + 2, message.length() - message.rfind("\r\n") - 2);
  if (_body != "") {
    logger.log("- Body parsed: [" + _body + "]");
  } else {
    logger.log("- No body found");
  }

  /**************************************************/
  /* End of parsing                                 */
  /**************************************************/

  logger.log("Request parsing finished");
}

/*
 * Getter for the request method
 */
HttpRequest::HttpMethod HttpRequest::get_method() const {
  return _method;
}

/*
 * Getter for the request URI
 */
std::string HttpRequest::get_uri() const {
  return _uri;
}

/*
 * Parse the method string into the method enum
 * @param method the method string
 * @return the method enum
 */
HttpRequest::HttpMethod HttpRequest::_parse_method(const std::string &method) {
  std::map<std::string, HttpMethod> method_map;

  method_map["GET"]    = GET;
  method_map["POST"]   = POST;
  method_map["DELETE"] = DELETE;

  return method_map.at(method);
}
