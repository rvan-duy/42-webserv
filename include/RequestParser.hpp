#pragma once

#include <HttpRequest.hpp>
#include <Server.hpp>
#include <Webserver.hpp>
#include <map>
#include <string>

class HttpRequest;

struct HttpHeaderData {
  HttpHeaderData() : chunked(false) {}
  HttpVersion                        httpVersion;
  std::string                        url;
  EHttpMethods                       method;
  std::string                        body;
  std::map<std::string, std::string> headers;
  bool                               chunked;
};

class RequestParser {
 public:
  static HttpRequest *parseHeader(std::string &rawRequest);
  static HttpRequest *processChunk(std::string &rawRequest);

 private:
  RequestParser();
  ~RequestParser();
};