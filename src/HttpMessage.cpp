#include "HttpMessage.hpp"

HttpMessage::HttpMessage() {}

HttpMessage::~HttpMessage() {}

/**************************************************/
/* Start of the getters                           */
/**************************************************/

/*
 * Getter for the HTTP version of the message
 */
HttpMessage::HttpVersion HttpMessage::get_version() const {
  return _version;
}

/*
 * Getter for the headers of the message
 */
std::map<std::string, std::string> HttpMessage::get_headers() const {
  return _headers;
}

/*
 * Getter for a specific header of the message
 * @param key the key of the header
 * @return the value of the header
 */

std::string HttpMessage::get_header(const std::string &key) const {
  return _headers.at(key);
}

/*
 * Getter for the body of the message
 */
std::string HttpMessage::get_body() const {
  return _body;
}

/**************************************************/
/* End of the getters                             */
/**************************************************/

/*
 * Parse the version string into the version enum
 * @param version the version string
 * @return the version enum
 */
HttpMessage::HttpVersion HttpMessage::parse_version(const std::string &version) {
  std::map<std::string, HttpVersion> version_map;

  version_map["HTTP/1.1"] = HTTP_1_1;
  version_map["HTTP/2.0"] = HTTP_2_0;
  version_map["HTTP/3.0"] = HTTP_3_0;

  return version_map.at(version);
}
