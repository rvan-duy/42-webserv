#ifndef HTTP_REQUEST_HPP
#define HTTP_REQUEST_HPP

#include <General.hpp>
#include <string>
#include <vector>

#include "HttpMessage.hpp"
#include "HttpResponse.hpp"
#include "Server.hpp"

std::string  extractArgument(const std::string& msg, int n);
EHttpMethods _parseMethod(const std::string& method);
std::string  _parseResponseStatus(const int& status);
bool         isMethodAllowed(Server& server, std::string uri, EHttpMethods method);

// HTTP REQUEST BASE
class HttpRequest : public HttpMessage {
 public:
  HttpRequest();
  HttpRequest(const std::string& msg);
  HttpRequest(const HttpRequest& obj);
  virtual ~HttpRequest();

  void extractInitialResponsLine(const std::string& msg);
  void extractHeaders(const std::string& msg);
  void extractBody(const std::string& msg);

  // Abstract
  virtual int          executeRequest(const Server& server)    = 0;
  virtual HttpResponse constructResponse(const Server& server) = 0;

  // Getters
  EHttpMethods getMethod() const;
  std::string  getUri() const;
  bool         getChunked() const;

 protected:
  EHttpMethods _method;
  std::string  _uri;
  bool         _chunked;
};

// GET

#define IS_DIR 1
#define IS_REG_FILE 2
#define IS_UNKNOWN 3

class GetRequest : public HttpRequest {
 public:
  GetRequest(std::string& msg);
  GetRequest(const GetRequest& ref);
  ~GetRequest();

  // Concrete
  int          executeRequest(const Server& server);
  HttpResponse constructResponse(const Server& server);

 private:
  // Private methods
  std::vector<std::string> _getAcceptedTypesFromHeader();
  std::string              _getPath(const Server& server);
  int                      _fileExists(const std::string& path);
};

// DELETE

class DeleteRequest : public HttpRequest {
 public:
  DeleteRequest(std::string& msg);
  DeleteRequest(const DeleteRequest& ref);
  ~DeleteRequest();

  // Concrete
  int          executeRequest(const Server& server);
  HttpResponse constructResponse(const Server& server);
};

// POST

class PostRequest : public HttpRequest {
 public:
  PostRequest(std::string& msg);
  PostRequest(const PostRequest& ref);
  ~PostRequest();

  // Concrete
  int          executeRequest(const Server& server);
  HttpResponse constructResponse(const Server& server);
};

// BAD

class BadRequest : public HttpRequest {
 public:
  BadRequest(std::string& msg);
  BadRequest(const BadRequest& ref);
  ~BadRequest();

  // Concrete
  int          executeRequest(const Server& server);
  HttpResponse constructResponse(const Server& server);
};

#endif  // HTTP_REQUEST_HPP
