#ifndef HTTP_RESPONSE_HPP
#define HTTP_RESPONSE_HPP

#include <sys/stat.h>

#include <fstream>
#include <iterator>
#include <sstream>

#include "HttpMessage.hpp"
#include "HttpRequest.hpp"

class HttpResponse : public HttpMessage {
 public:
  HttpResponse();
  HttpResponse(const HttpResponse &obj);
  HttpResponse(HttpVersion version, int statusCode, std::string statusMessage);
  ~HttpResponse();

  // Methods
  // void        createResponse(const HttpRequest &request, const std::string &root, const std::string &index);
  std::string toStr() const;

  // Getters
  int         getStatusCode() const;
  std::string getStatusMessage() const;

  // Setters
  void        _setResponse(const std::string &path, const int status_code, const std::string &status_message,
                            const HttpVersion version);

 private:
  int         _statusCode;     // status code of response
  std::string _statusMessage;  // status message of response

  std::string _getContentType(const std::string &path) const;
};

#endif  // HTTP_RESPONSE_HPP
