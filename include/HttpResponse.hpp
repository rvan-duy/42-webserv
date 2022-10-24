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
  int         _status_code;     // status code of response
  std::string _status_message;  // status message of response
};

#endif  // HTTP_RESPONSE_HPP
