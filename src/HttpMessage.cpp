#include "HttpMessage.hpp"

HttpMessage::HttpMessage(){}

HttpMessage::~HttpMessage(){}

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
