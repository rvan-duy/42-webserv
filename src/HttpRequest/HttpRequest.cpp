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
  _chunked = obj._chunked;
}

HttpRequest::~HttpRequest() {}

/*
 * Protected methods
 */

bool HttpRequest::_isMethodAllowed(const std::map<EHttpMethods, bool> allowedMethods) const {
  EHttpMethods currentMethod = _method;

  for (std::map<EHttpMethods, bool>::const_iterator it = allowedMethods.begin(); it != allowedMethods.end(); it++) {
    if (it->first == currentMethod) {
      return it->second;
    }
  }
  return false;
}

/*
//  * Parse the method string into the method enum
//  * @param method the method string
//  * @return the method enum
//  */
// EHttpMethods _parseMethod(const std::string &method) {
//   Logger &logger = Logger::getInstance();
//   std::map<std::string, EHttpMethods> method_map;

//   method_map["GET"]    = GET;
//   method_map["POST"]   = POST;
//   method_map["DELETE"] = DELETE;

//   try {
//     return method_map.at(method);
//   } catch (std::exception& e) {
//     // _status_code = 400;
//     logger.error("bad request: method unsupported ( " + method + " )");
//     return NONE;
//   }
// }

// /* This needs to include all the response statuses used in executeRequest */
// std::string _parseResponseStatus(const int &status) {
//   std::map<int, std::string> response_map;

//   response_map[403]   = "Forbidden";
//   response_map[404]   = "Not found";
//   response_map[405]   = "Method not alowed";
//   response_map[409]   = "Conflict";

//   return response_map.at(status);
// }
