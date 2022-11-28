#ifndef HTTP_MESSAGE_HPP
#define HTTP_MESSAGE_HPP

#include <iostream>
#include <map>

enum HttpVersion
{
  HTTP_1_1,
  HTTP_2_0,
  HTTP_3_0
};

class HttpMessage
{
public:
  HttpMessage();
  HttpMessage(HttpVersion &verion);
  virtual ~HttpMessage();

  // Getters
  HttpVersion getVersion() const;
  std::string getVersionToStr() const;
  std::map<std::string, std::string> getHeaders() const;
  std::string getHeader(const std::string &key) const;
  std::string getBody() const;

protected:
  HttpVersion _version;                        // HTTP version of message
  std::map<std::string, std::string> _headers; // headers of message
  std::string _body;                           // body of message

  HttpVersion _parseVersion(const std::string &version);
};

#endif // HTTP_MESSAGE_HPP
