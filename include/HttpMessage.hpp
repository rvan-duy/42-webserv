#ifndef HTTP_MESSAGE_HPP
#define HTTP_MESSAGE_HPP

#include <Webserver.hpp>
#include <iostream>
#include <map>

class HttpMessage {
 public:
  HttpMessage();
  HttpMessage(std::map<std::string, std::string> const &headers,
              HttpVersion const &version, std::string const &body);
  HttpMessage(std::map<std::string, std::string> const &headers,
              HttpVersion const &version);
  HttpMessage(HttpVersion const &version);
  HttpMessage(HttpMessage const &other);
  virtual ~HttpMessage();

  // Getters
  HttpVersion getVersion() const;
  std::string getVersionToStr() const;
  std::map<std::string, std::string> getHeaders() const;
  std::string getHeader(const std::string &key) const;
  std::string getBody() const;

 protected:
  void setHeader(std::string const &key, std::string const &value);
  void setBody(std::string const &body);

  HttpVersion _version;                         // HTTP version of message
  std::map<std::string, std::string> _headers;  // headers of message
  std::string _body;                            // body of message
};

#endif  // HTTP_MESSAGE_HPP
