#ifndef HTTP_MESSAGE_HPP
#define HTTP_MESSAGE_HPP

#include <iostream>
#include <map>

class HttpMessage {
 public:
  HttpMessage();
  virtual ~HttpMessage();

  enum HttpVersion { HTTP_1_1, HTTP_2_0, HTTP_3_0 };

  // Getters
  HttpVersion                        get_version() const;
  std::string                        get_version_to_str() const;
  std::map<std::string, std::string> get_headers() const;
  std::string                        get_header(const std::string &key) const;
  std::string                        get_body() const;

 protected:
  HttpVersion                        _version;  // HTTP version of message
  std::map<std::string, std::string> _headers;  // headers of message
  std::string                        _body;     // body of message

  HttpVersion                        _parse_version(const std::string &version);
};

#endif  // HTTP_MESSAGE_HPP
