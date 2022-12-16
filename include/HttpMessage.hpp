#ifndef HTTP_MESSAGE_HPP
#define HTTP_MESSAGE_HPP

#include <iostream>
#include <map>

enum class HTTPStatusCode {
  OK = 200,
  BAD_REQUEST = 400,
  NOT_FOUND = 404,
  METHOD_NOT_ALLOWED = 405,
  NOT_ACCEPTABLE = 406,
  CONTENT_TOO_LARGE = 413,
  I_AM_A_TEAPOT = 418,
  INTERNAL_SERVER_ERROR = 500
};

class HttpMessage {
 public:

  enum HttpVersion { HTTP_1_1, HTTP_2_0, HTTP_3_0 };

  HttpMessage();
  HttpMessage(HttpVersion& verion);
  virtual ~HttpMessage();

  // Getters
  HttpVersion                        getVersion() const;
  std::string                        getVersionToStr() const;
  std::map<std::string, std::string> getHeaders() const;
  std::string                        getHeader(const std::string &key) const;
  std::string                        getBody() const;

 protected:
  HttpVersion                        _version;  // HTTP version of message
  std::map<std::string, std::string> _headers;  // headers of message
  std::string                        _body;     // body of message

  HttpVersion                        _parseVersion(const std::string &version);
};

#endif  // HTTP_MESSAGE_HPP
