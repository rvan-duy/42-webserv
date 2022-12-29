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

enum class FileType { DIR, FILE, NOT_FOUND };

class HttpRequest : public HttpMessage {
 public:
  HttpRequest();
  HttpRequest(HttpHeaderData const &data);
  HttpRequest(const HttpRequest &obj);
  virtual ~HttpRequest();

  EHttpMethods getMethod() const;
  HTTPStatusCode getStatus() const;

  HttpRequest *operator+(const HttpRequest &other);
  void unChunkBody();
  bool isFirstChunk();

  virtual HttpResponse executeRequest(const Server &server);

 protected:
  EHttpMethods _method;
  std::string _uri;
  // TODO: remove default value
  HTTPStatusCode _statusCode = HTTPStatusCode::NOT_SET;

  /* Request handling */
  virtual HttpResponse _handleCgiRequest(std::string const &path,
                                         Route const &route) const;
  HttpResponse _handleFileRequest(std::string const &path, const Route &route,
                                  const FileType &type) const;

  /* Response constructors */
  virtual HttpResponse _errorResponse(HTTPStatusCode const &statusCode,
                                      Route const &route) const;
  HttpResponse _responseWithFile(std::string const &path,
                                 HTTPStatusCode statusCode) const;
  HttpResponse _responseWithBody(std::map<std::string, std::string> headers,
                                 std::string body) const;

  /* Helpers */
  FileType _getFileType(const std::string &path) const;
  bool _isTypeAccepted() const;
  std::vector<std::string> _getPossiblePaths(
      const std::string &path,
      const std::vector<std::string> &index_files) const;
};

// GET
class GetRequest : public HttpRequest {
 public:
  GetRequest(HttpHeaderData const &data);
  GetRequest(const GetRequest &ref);
  ~GetRequest();

 private:
  // Private methods
  HttpResponse _handleCgiRequest(std::string const &path,
                                 Route const &route) const;
  HttpResponse _errorResponse(HTTPStatusCode const &statusCode,
                              Route const &route) const;
  std::string _getErrorPage(const Route &route, HTTPStatusCode errorCode) const;
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
