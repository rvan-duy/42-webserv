#ifndef HTTP_REQUEST_HPP
#define HTTP_REQUEST_HPP

#include <Webserver.hpp>
#include <StatusCodes.hpp>
#include "HttpMessage.hpp"
#include <RequestParser.hpp>
#include "HttpResponse.hpp"
#include <Server.hpp>
#include <string>

class Server;
class HttpResponse;

struct HttpHeaderData;
// bool isMethodAllowed(Server &server, std::string uri, EHttpMethods method);

// HTTP REQUEST BASE
class HttpRequest : public HttpMessage
{
public:
  HttpRequest(HttpHeaderData const &data);
  HttpRequest();
  HttpRequest(const HttpRequest &obj);
  virtual ~HttpRequest();

  // Abstract
  virtual HTTPStatusCode executeRequest(Server &server) = 0;
  virtual HttpResponse constructResponse(Server &server, std::string &index) = 0;

  // Getters
  EHttpMethods getMethod() const;
  std::string getUrl() const;
  bool getChunked() const;

protected:
  bool isMethodAllowed(Server &server, std::string uri, EHttpMethods method);

  EHttpMethods _method;
  std::string _url;
  bool _chunked;
};

// GET
class GetRequest : public HttpRequest
{
public:
  GetRequest(HttpHeaderData const &data);
  GetRequest(const GetRequest &ref);
  ~GetRequest();

  // Concrete
  HTTPStatusCode executeRequest(Server &server);
  HttpResponse constructResponse(Server &server, std::string &index);
};

// DELETE

class DeleteRequest : public HttpRequest
{
public:
  DeleteRequest(HttpHeaderData const &data);
  DeleteRequest(const DeleteRequest &ref);
  ~DeleteRequest();

  // Concrete
  HTTPStatusCode executeRequest(Server &server);
  HttpResponse constructResponse(Server &server, std::string &index);
};

// POST

class PostRequest : public HttpRequest
{
public:
  PostRequest(HttpHeaderData const &data);
  PostRequest(const PostRequest &ref);
  ~PostRequest();

  // Concrete
  HTTPStatusCode executeRequest(Server &server);
  HttpResponse constructResponse(Server &server, std::string &index);
};

class BadRequest : public HttpRequest
{
public:
  BadRequest(HTTPStatusCode _statusCode);
  BadRequest(const BadRequest &ref);
  ~BadRequest();

  // Concrete
  HTTPStatusCode executeRequest(Server &server);
  HttpResponse constructResponse(Server &server, std::string &index);

private:
  std::string getErrorMessage() const;
  HTTPStatusCode _statusCode;
};

#endif // HTTP_REQUEST_HPP
