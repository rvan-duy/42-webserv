#include "HttpRequest.hpp"

GetRequest::GetRequest(std::string& msg) : HttpRequest(msg) {}

GetRequest::GetRequest(const GetRequest& ref) : HttpRequest(ref)  {}

GetRequest::~GetRequest() {}

int GetRequest::executeRequest(Server& server) {
  (void)server;
  return 0;
}

HttpResponse GetRequest::constructResponse(Server& server, std::string& index) {
  Logger &logger = Logger::getInstance();
  logger.log("Response type: GET");
  (void)server;
  HttpResponse response;

  std::string path = "root" + getUri();  // future get root from server class
  logger.log("Path: " + path);

  if (!_fileExists(path)) {
    logger.log("File doesn't exist");

    const std::string index_path = path + index;
    logger.log("Looking for index path: " + index_path);
    if (!_fileExists(index_path)) {
      logger.log("Index file doesn't exist");

      response._setResponse("root/404/index.html", 404, "Not Found", getVersion()); // very hardcoded lol
      return response;
    } else {
      logger.log("Index file exists");
      path = index_path;
    }
  }
  response._setResponse(path, 200, "OK", getVersion());
  return response;
}
