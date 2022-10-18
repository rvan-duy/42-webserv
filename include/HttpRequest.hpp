#ifndef HTTP_REQUEST_HPP
#define HTTP_REQUEST_HPP

#include "HttpMessage.hpp"

class HttpRequest : public HttpMessage {
 public:
  HttpRequest();
  ~HttpRequest();

  // Methods
  void parse(const std::string &message);

 private:
  int         _status_code;     // status code of response
  std::string _status_message;  // status message of response
};

#endif  // HTTP_REQUEST_HPP
