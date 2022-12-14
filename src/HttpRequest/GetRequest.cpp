#include "HttpRequest.hpp"

GetRequest::GetRequest(std::string& msg) : HttpRequest(msg) {}

GetRequest::GetRequest(const GetRequest& ref) : HttpRequest(ref)  {}

GetRequest::~GetRequest() {}

int GetRequest::executeRequest(const Server& server) {
  (void)server;
  return 0;
}

// Steps the function needs to take
// 1. Determine the status code
// - 200 OK
// - 404 Not Found
// - 500 Internal Server Error
// 2. Determine the headers of the response, content length, date
// 3. Determine the bod of the response, HTML Code, image etc

HttpResponse GetRequest::constructResponse(const Server& server) {
  Logger &logger = Logger::getInstance();
  logger.log("Response type: GET");

  HttpResponse response;
  
}

// HttpResponse GetRequest::constructResponse(Server& server, std::string& index) {
//   Logger &logger = Logger::getInstance();
//   logger.log("Response type: GET");
//   (void)server;
//   HttpResponse response;

//   std::string path = server.getRoot(getUri()) + getUri();  // future get root from server class
//   logger.debug("root: " + server.getRoot(getUri())); // the future is now
//   logger.log("Path: " + path);

//   if (!_fileExists(path)) {
//     logger.log("File doesn't exist");

//     const std::string index_path = path + index;
//     logger.log("Looking for index path: " + index_path);
//     if (!_fileExists(index_path)) {
//       logger.log("Index file doesn't exist");

//       response._setResponse("root/404/index.html", 404, "Not Found", getVersion()); // very hardcoded lol
//       return response;
//     } else {
//       logger.log("Index file exists");
//       path = index_path;
//     }
//   }
//   response._setResponse(path, 200, "OK", getVersion());
//   return response;
// }
