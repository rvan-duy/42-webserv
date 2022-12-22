#ifndef HTTP_RESPONSE_HPP
#define HTTP_RESPONSE_HPP

#include <sys/stat.h>

#include <StatusCodes.hpp>
#include <Webserver.hpp>
#include <fstream>
#include <iterator>
#include <sstream>

#include "HttpMessage.hpp"
#include "HttpRequest.hpp"

class HttpResponse : public HttpMessage {
 public:
  HttpResponse(HTTPStatusCode status);
  HttpResponse(HTTPStatusCode status, std::map<std::string, std::string> const &headers, std::string const &body);
  HttpResponse();
  HttpResponse(const HttpResponse &obj);
  ~HttpResponse();

  // Methods
  std::string toStr() const;

 private:
  HTTPStatusCode _statusCode;     // status code of response
  std::string    _statusMessage;  // status message of response
};

#endif  // HTTP_RESPONSE_HPP
