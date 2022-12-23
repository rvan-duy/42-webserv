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

enum class FileType { IS_DIR, IS_REG_FILE, IS_UNKNOWN };

// HTTP REQUEST BASE
class HttpRequest : public HttpMessage {
 public:
  HttpRequest();
  HttpRequest(HttpHeaderData const &data);
  HttpRequest(const HttpRequest &obj);
  virtual ~HttpRequest();

  virtual HttpResponse constructResponse(const Server &server) = 0;

 protected:
  EHttpMethods _method;
  std::string _uri;
  bool _chunked;

  bool _isMethodAllowed(
      const std::map<EHttpMethods, bool> allowedMethods) const;
  bool _isTypeAccepted() const;
  std::vector<std::string> _getPossiblePaths(
      const std::string &path,
      const std::vector<std::string> &index_files) const;
  FileType _getFileType(const std::string &path) const;
  std::string _getErrorPageIndex(const Route &route,
                                 HTTPStatusCode errorCode) const;
  HttpResponse _errorResponseWithFile(HTTPStatusCode const &statusCode,
                                      Route const &route) const;
  HttpResponse _responseWithFile(std::string const &path,
                                 HTTPStatusCode statusCode) const;
  HttpResponse _responseWithBody(std::map<std::string, std::string> headers,
                                 std::string body) const;
  std::string _constructPath(const std::string &root) const;
  virtual HttpResponse _handleCgiRequest(std::string const &path,
                                         Route const &route) const;
  bool _isCgiRequest(std::string path) const;
  HttpResponse _handleFileRequest(std::string const &path,
                                  Route const &route) const;
};

// GET
class GetRequest : public HttpRequest {
 public:
  GetRequest(HttpHeaderData const &data);
  GetRequest(const GetRequest &ref);
  ~GetRequest();

  HttpResponse constructResponse(const Server &server);

 private:
  // Private methods
  HttpResponse _handleCgiRequest(std::string const &path,
                                 Route const &route) const;
  HttpResponse _handleFileRequest(std::string const &path,
                                  Route const &route) const;
};

// DELETE
class DeleteRequest : public HttpRequest {
 public:
  DeleteRequest(HttpHeaderData const &data);
  DeleteRequest(const DeleteRequest &ref);
  ~DeleteRequest();

  HttpResponse constructResponse(const Server &server);
};

// POST
class PostRequest : public HttpRequest {
 public:
  PostRequest(HttpHeaderData const &data);
  PostRequest(const PostRequest &ref);
  ~PostRequest();

  HttpResponse constructResponse(const Server &server);
};

// BAD
class BadRequest : public HttpRequest {
 public:
  BadRequest(HTTPStatusCode statusCode);
  BadRequest(const BadRequest &ref);
  ~BadRequest();

  HttpResponse constructResponse(const Server &server);

 private:
  HTTPStatusCode _statusCode;
};

#endif  // HTTP_REQUEST_HPP
