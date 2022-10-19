#ifndef HTTP_REQUEST_HPP
#define HTTP_REQUEST_HPP

#include "HttpMessage.hpp"

class HttpRequest : public HttpMessage {
 public:
  HttpRequest();
  HttpRequest(const HttpRequest &obj);
  ~HttpRequest();

  enum HttpMethod { GET, POST, DELETE };

  // Methods
  void parse(const std::string &message);

  // Getters
  HttpMethod  get_method() const;
  std::string get_uri() const;

 private:
  HttpMethod  _method;  // HTTP method of request
  std::string _uri;     // URI of request

  HttpMethod  parse_method(const std::string &method);
};

#endif  // HTTP_REQUEST_HPP
