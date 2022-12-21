#ifndef HTTP_MESSAGE_HPP
#define HTTP_MESSAGE_HPP

#include <Webserver.hpp>
#include <iostream>
#include <map>

class HttpMessage {
 public:
  HttpMessage();
  HttpMessage(HttpVersion const &version, std::map<std::string, std::string> const &headers, std::string const &body);
  HttpMessage(HttpVersion const &version);
  HttpMessage(HttpMessage const &other);
  virtual ~HttpMessage();

  // Getters
  HttpVersion                        getVersion() const;
  std::string                        getVersionToStr() const;
  std::map<std::string, std::string> getHeaders() const;
  std::string                        getHeader(const std::string &key) const;
  std::string                        getBody() const;
  void                               setBody(std::string const &body);
  void                               setHeader(std::string const &key, std::string const &value);

 protected:
  HttpVersion                        _version;  // HTTP version of message
  std::map<std::string, std::string> _headers;  // headers of message
  std::string                        _body;     // body of message
};

#endif  // HTTP_MESSAGE_HPP
