#ifndef HTTP_RESPONSE_HPP
#define HTTP_RESPONSE_HPP

#include "HttpMessage.hpp"

class HttpResponse : public HttpMessage {
 public:
  HttpResponse();
  ~HttpResponse();

  // Methods
  void parse(const std::string &message);

  enum HttpMethod { GET, POST, DELETE };

 private:
  HttpMethod  _method;  // HTTP method of request
  std::string _uri;     // URI of request
};

#endif  // HTTP_RESPONSE_HPP
