#ifndef HTTP_REQUEST_HPP
#define HTTP_REQUEST_HPP

#include "HttpMessage.hpp"
#include "HttpResponse.hpp"
#include "Server.hpp"
#include <string>

// HTTP REQUEST BASE

class HttpRequest : public HttpMessage {
 public:
  HttpRequest();
  HttpRequest(const HttpRequest &obj);
  virtual ~HttpRequest();

  enum HttpMethod { GET, POST, DELETE };

  // Abstract
  virtual void executeRequest() = 0;
  virtual HttpResponse constructResponse(Server& server, std::string& index) = 0;

  // Getters
  HttpMethod  getMethod() const;
  std::string getUri() const;
  bool        getChunked() const;

 private:
  HttpMethod                         _method;
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
  void executeRequest();
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
  void executeRequest();
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
  void executeRequest();
  HttpResponse constructResponse(Server& server, std::string& index);
};

#endif  // HTTP_REQUEST_HPP
