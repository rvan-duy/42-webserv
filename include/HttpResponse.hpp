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
  ~HttpResponse();

  // Methods
  void        create_response(const HttpRequest &request, const std::string &root, const std::string &index);
  std::string to_str() const;

  // Getters
  int         get_status_code() const;
  std::string get_status_message() const;

  // Set HttpResponse to hardcoded responses
  void set_to_404_page(const std::string &message);

 private:
  int         _status_code;     // status code of response
  std::string _status_message;  // status message of response

  void        _set_response(const std::string &path, const int status_code, const std::string &status_message, const HttpVersion version);
  bool        _file_exists(const std::string &path) const;
  std::string _get_content_type(const std::string &path) const;
};

#endif  // HTTP_RESPONSE_HPP
