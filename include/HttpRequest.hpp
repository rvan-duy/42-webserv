#ifndef HTTP_REQUEST_HPP
#define HTTP_REQUEST_HPP

#include "HttpMessage.hpp"
#include "HttpResponse.hpp"
#include <string>

// HTTP REQUEST BASE

class HttpRequest : public HttpMessage {
 public:
  HttpRequest();
// maybe equal overload?
  HttpRequest(const HttpRequest &obj);
  virtual ~HttpRequest();

  // Abstract
  void executeRequest() = 0;
  HttpResponse& constructResponse() = 0;

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
	GetRequest();
	GetRequest(const GetRequest& ref);
	GetRequest& operator=(const GetRequest& ref);
	~GetRequest();

	// Concrete
  void executeRequest();
  HttpResponse& constructResponse();
};

// DELETE

class	DeleteRequest : public HttpRequest
{
public:
	DeleteRequest();
	DeleteRequest(const DeleteRequest& ref);
	DeleteRequest& operator=(const DeleteRequest& ref);
	~DeleteRequest();

	// Concrete
  void executeRequest();
  HttpResponse& constructResponse();
};

// POST

class	PostRequest : public HttpRequest
{
public:
	PostRequest();
	PostRequest(const PostRequest& ref);
	PostRequest& operator=(const PostRequest& ref);
	~PostRequest();

	// Concrete
  void executeRequest();
  HttpResponse& constructResponse();
};

#endif  // HTTP_REQUEST_HPP
