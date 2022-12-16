#ifndef HTTP_REQUEST_HPP
#define HTTP_REQUEST_HPP

#include <General.hpp>
#include <string>
#include <vector>

#include "HttpMessage.hpp"
#include "HttpResponse.hpp"
#include "Server.hpp"

struct Route;  // Forward declaration

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

 protected:
  EHttpMethods _method;
  std::string  _uri;
  bool         _chunked;

  // Protected methods
  bool _isMethodAllowed(const std::map<EHttpMethods, bool> allowedMethods) const;
};

// GET

enum class FileType {
  IS_DIR,
  IS_REG_FILE,
  IS_UNKNOWN
};

// # define IS_DIR      0
// # define IS_REG_FILE 1
// # define IS_UNKNOWN  2

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
  std::string              _getErrorPageIndex(const Route& route, HTTPStatusCode errorCode) const;
  std::vector<std::string> _getPossiblePaths(const std::string& path, const std::vector<std::string>& index_files);
  std::vector<std::string> _getAcceptedTypesFromHeader();
  std::string              _constructPath(const std::string& root) const;
  FileType                 _fileExists(const std::string& path) const;
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
