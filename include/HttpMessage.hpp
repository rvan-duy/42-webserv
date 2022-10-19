#ifndef HTTP_MESSAGE_HPP
#define HTTP_MESSAGE_HPP

#include <iostream>
#include <map>

class HttpMessage {
 public:
  HttpMessage();
  virtual ~HttpMessage();

  // Methods
  virtual void parse(const std::string &message) = 0;

  enum HttpVersion { HTTP_1_1, HTTP_2_0, HTTP_3_0 };

 protected:
  HttpVersion                        _version;  // HTTP version of message
  std::map<std::string, std::string> _headers;  // headers of message
  std::string                        _body;     // body of message

  HttpVersion parse_version(const std::string &version);
};

#endif  // HTTP_MESSAGE_HPP
