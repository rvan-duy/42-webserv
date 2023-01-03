#ifndef HTTP_REQUEST_HPP
#define HTTP_REQUEST_HPP

#include <CGI.hpp>
#include <RequestParser.hpp>
#include <Server.hpp>
#include <StatusCodes.hpp>
#include <Webserver.hpp>
#include <array>
#include <string>
#include <vector>

#include "HttpMessage.hpp"
#include "HttpResponse.hpp"

class Server;
class HttpResponse;
struct Route;
struct HttpHeaderData;

enum class FileType { DIRECTORY, PYTHON_SCRIPT, REGULAR_FILE, NOT_FOUND };
#define MethodMap std::map<EHttpMethods, bool>
#define HeaderMap std::map<std::string, std::string>

// HTTP REQUEST BASE
class HttpRequest : public HttpMessage {
 public:
  HttpRequest();
  HttpRequest(const HttpHeaderData &data);
  HttpRequest(const HttpRequest &ref);
  HttpRequest &operator=(const HttpRequest &ref);
  virtual ~HttpRequest();

  virtual HttpResponse executeRequest(const Server &server);

  HttpRequest         *operator+(const HttpRequest &chunk);
  void                 unChunkBody();
  bool                 isFirstChunk();

  EHttpMethods         getMethod() const;
  HTTPStatusCode       getStatus() const;

 protected:
  bool           isMethodAllowed(const MethodMap allowedMethods, const EHttpMethods &method) const;
  FileType       getFileType(const std::string &path) const;
  std::string    constructFullPath(const std::string &rootDirectory, const std::string &uri) const;
  std::string    fileToStr(std::ifstream &file) const;
  std::string    getContentType(const std::string &path) const;
  std::string    getFileSize(std::ifstream &file) const;
  HttpResponse   createCGIResponse(const Route &route, const std::string &path) const;
  HttpResponse   responseWithBody(HeaderMap headers, std::string body) const;
  HttpResponse   responseWithFile(const std::string &path, HTTPStatusCode statusCode) const;

  std::string    _uri;
  EHttpMethods   _method;
  HTTPStatusCode _statusCode = HTTPStatusCode::NOT_SET;
};

// GET
class GetRequest : public HttpRequest {
 public:
  GetRequest();
  GetRequest(const HttpHeaderData &data);
  GetRequest(const GetRequest &ref);
  GetRequest &operator=(const GetRequest &ref);
  ~GetRequest();

  HttpResponse executeRequest(const Server &server);

 private:
  bool                     _isTypeAccepted() const;
  std::string              _getErrorPage(const Route &route, HTTPStatusCode errorCode) const;
  HttpResponse             _createRedirectionResponse(const Route &route) const;
  HttpResponse             _createDirectoryResponse(const Route &route, const std::string &path) const;
  HttpResponse             _createFileResponse(const Route &route, const std::string &path) const;
  HttpResponse             _errorResponseWithHtml(HTTPStatusCode statusCode, Route const &route) const;
  std::vector<std::string> _constructPossiblePaths(const std::string              &path,
                                                   const std::vector<std::string> &indexFiles) const;
};

// DELETE
class DeleteRequest : public HttpRequest {
 public:
  DeleteRequest(HttpHeaderData const &data);
  DeleteRequest(const DeleteRequest &ref);
  ~DeleteRequest();
};

// POST
class PostRequest : public HttpRequest {
 public:
  PostRequest(HttpHeaderData const &data);
  PostRequest(const PostRequest &ref);
  ~PostRequest();
};

// BAD
class BadRequest : public HttpRequest {
 public:
  BadRequest(HTTPStatusCode statusCode);
  BadRequest(const BadRequest &ref);
  ~BadRequest();

  HttpResponse executeRequest(const Server &server);
};

#endif  // HTTP_REQUEST_HPP