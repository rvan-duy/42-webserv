#ifndef HTTP_REQUEST_HPP
#define HTTP_REQUEST_HPP

#include <General.hpp>

#include "HttpMessage.hpp"
#include "HttpResponse.hpp"
#include "Server.hpp"
#include <string>

std::string  extractArgument(const std::string& msg, int n);
EHttpMethods _parseMethod(const std::string &method);
std::string _parseResponseStatus(const int &status);
bool  isMethodAllowed(Server& server, std::string uri, EHttpMethods method);

// HTTP REQUEST BASE
class HttpRequest : public HttpMessage {
 public:
  HttpRequest();
  HttpRequest(const std::string& msg);
  HttpRequest(const HttpRequest &obj);
  virtual ~HttpRequest();

  // enum HttpMethod { GET, POST, DELETE, NONE };

  void  extractInitialResponsLine(const std::string& msg);
  void  extractHeaders(const std::string& msg);
  void  extractBody(const std::string& msg);


  // Abstract
  virtual int executeRequest(Server& server) = 0;
  virtual HttpResponse constructResponse(Server& server, std::string& index) = 0;

  // Getters
  EHttpMethods  getMethod() const;
  std::string   getUri() const;
  bool          getChunked() const;

 protected:
  EHttpMethods                       _method;
  std::string                        _uri;
  bool                               _chunked;
};

// GET

class	GetRequest : public HttpRequest
{
public:
	GetRequest(std::string& msg);
	GetRequest(const GetRequest& ref);
	~GetRequest();

	// Concrete
  int executeRequest(Server& server);
  HttpResponse constructResponse(Server& server, std::string& index);
};

// DELETE

class	DeleteRequest : public HttpRequest
{
public:
	DeleteRequest(std::string& msg);
	DeleteRequest(const DeleteRequest& ref);
	~DeleteRequest();

	// Concrete
  int executeRequest(Server& server);
  HttpResponse constructResponse(Server& server, std::string& index);
};

// POST

class	PostRequest : public HttpRequest
{
public:
	PostRequest(std::string& msg);
	PostRequest(const PostRequest& ref);
	~PostRequest();

	// Concrete
  int executeRequest(Server& server);
  HttpResponse constructResponse(Server& server, std::string& index);
};

// BAD

class	BadRequest : public HttpRequest
{
public:
	BadRequest(std::string& msg);
	BadRequest(const BadRequest& ref);
	~BadRequest();

	// Concrete
  int executeRequest(Server& server);
  HttpResponse constructResponse(Server& server, std::string& index);
};

#endif  // HTTP_REQUEST_HPP
