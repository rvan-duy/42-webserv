#ifndef HTTP_RESPONSE_HPP
#define HTTP_RESPONSE_HPP

#include <sys/stat.h>

#include <Webserver.hpp>
#include <StatusCodes.hpp>
#include <fstream>
#include <iterator>
#include <sstream>

#include "HttpMessage.hpp"
#include "HttpRequest.hpp"

class HttpResponse : public HttpMessage {
 public:
  HttpResponse();
  HttpResponse(const HttpResponse &obj);
  ~HttpResponse();

  // Methods
  std::string toStr() const;

  // Setters
  void _setResponse(const std::string &path, HTTPStatusCode statusCode, const std::string &status_message,
                    const HttpVersion version);

 private:
  HTTPStatusCode _statusCode;     // status code of response
  std::string    _statusMessage;  // status message of response

  std::string    _getContentType(const std::string &path) const;
};

#endif  // HTTP_RESPONSE_HPP
