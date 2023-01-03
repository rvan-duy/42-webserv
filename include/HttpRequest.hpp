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

enum class FileType { DIRECTORY, PYTHON_SCRIPT, FILE, NOT_FOUND };
#define MethodMap std::map<EHttpMethods, bool>
#define HeaderMap std::map<std::string, std::string>

class HttpRequest : public HttpMessage {
 public:
  HttpRequest();
  HttpRequest(const HttpHeaderData &data);
  HttpRequest(const HttpRequest &ref);
  HttpRequest &operator=(const HttpRequest &ref);
  HttpRequest *operator+(const HttpRequest &other);
  virtual ~HttpRequest();

  EHttpMethods getMethod() const;
  HTTPStatusCode getStatus() const;

  void unChunkBody();
  bool isFirstChunk();

  virtual HttpResponse executeRequest(const Server &server);

  std::string constructFullPath(const std::string &rootDirectory, const std::string &uri) const;
  HttpResponse createCGIResponse(const Route &route, const std::string &path) const;
  std::string getFileSize(std::ifstream &file) const;
  std::string getContentType(const std::string &path) const;

 protected:
  EHttpMethods _method;
  std::string _uri;
  // TODO: remove default value
  HTTPStatusCode _statusCode = HTTPStatusCode::NOT_SET;

  /* Request handling */
  // virtual HttpResponse _handleCgiRequest(std::string const 
  HttpResponse _handleFileRequest(std::string const &path, const Route &route,
                                  const FileType &type) const;

  /* Response constructors */
  // virtual HttpResponse _errorResponse(HTTPStatusCode const &statusCode,
  //                                       Route const &route) const;
  HttpResponse _responseWithFile(std::string const &path,
                                 HTTPStatusCode statusCode) const;
  HttpResponse _responseWithBody(std::map<std::string, std::string> headers,
                                 std::string body) const;

  /* Helpers */
  FileType _getFileType(const std::string &path) const;
  bool _isTypeAccepted() const;
  // std::vector<std::string> _getPossiblePaths(
  //     const std::string &path,
  //     const std::vector<std::string> &index_files) const;
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
  HttpResponse _errorResponse(HTTPStatusCode const &statusCode,
                                        Route const &route) const;
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

bool isMethodAllowed(const std::map<EHttpMethods, bool> allowedMethods, EHttpMethods method);

#endif  // HTTP_REQUEST_HPP
