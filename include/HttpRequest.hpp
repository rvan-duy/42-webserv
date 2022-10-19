#ifndef HTTP_REQUEST_HPP
#define HTTP_REQUEST_HPP

#include "HttpMessage.hpp"

class HttpRequest : public HttpMessage {
 public:
  HttpRequest();
  ~HttpRequest();

  // Methods
  void parse(const std::string &message);

  enum HttpMethod { GET, POST, DELETE };

 private:
  HttpMethod  _method;          // HTTP method of request
  std::string _uri;             // URI of request
};

#endif  // HTTP_REQUEST_HPP
