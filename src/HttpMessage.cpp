#include "HttpMessage.hpp"

HttpMessage::HttpMessage() : _version(HTTP_1_1) {}

HttpMessage::HttpMessage(HttpVersion const &version,
                         std::map<std::string, std::string> const &headers,
                         std::string const &body)
    : _version(version), _headers(headers), _body(body) {}

HttpMessage::HttpMessage(HttpVersion const &version) : _version(version) {}

HttpMessage::HttpMessage(HttpMessage const &other)
    : _version(other._version), _headers(other._headers), _body(other._body) {}

HttpMessage::~HttpMessage() {}

/**************************************************/
/* Start of the getters                           */
/**************************************************/

/*
 * Getter for the HTTP version of the message
 */
HttpVersion HttpMessage::getVersion() const { return _version; }

/*
 * Getter for the HTTP version of the message as a string
 */
std::string HttpMessage::getVersionToStr() const {
  switch (_version) {
    case HTTP_1_1:
      return "HTTP/1.1";
    case HTTP_2_0:
      return "HTTP/2.0";
    case HTTP_3_0:
      return "HTTP/3.0";
    default:
      return "HTTP/1.1";
  }
}
/*
 * Getter for the headers of the message
 */
std::map<std::string, std::string> HttpMessage::getHeaders() const {
  return _headers;
}

/*
 * Getter for a specific header of the message
 * @param key the key of the header
 * @return the value of the header
 */
std::string HttpMessage::getHeader(const std::string &key) const {
  std::map<std::string, std::string>::const_iterator location =
      _headers.find(key);
  if (location == _headers.end()) return "";
  return location->second;
}

size_t HttpMessage::getIntHeader(const std::string &key) const {
  std::string header = _headers.at(key);
  try {
    return std::stoi(header);
  } catch (std::exception e) {
    return SIZE_MAX;
  }
}

bool HttpMessage::hasHeader(const std::string key) const {
  return _headers.count(key) > 0 ? true : false;
}

/*
 * Getter for the body of the message
 */
std::string HttpMessage::getBody() const { return _body; }

/**************************************************/
/* End of the getters                             */
/**************************************************/

void HttpMessage::setHeader(std::string const &key, std::string const &value) {
  _headers[key] = value;
}

// TODO: check max body size
void HttpMessage::addBody(std::string const &body) { _body += body; }
