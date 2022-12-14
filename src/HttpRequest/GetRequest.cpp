#include "HttpRequest.hpp"

GetRequest::GetRequest(std::string& msg) : HttpRequest(msg) {}

GetRequest::GetRequest(const GetRequest& ref) : HttpRequest(ref) {}

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
  Logger& logger = Logger::getInstance();
  logger.log("[RESPONSE-BUILDING] GetRequest: Response type: GET");
  HttpResponse response;

  std::string  path = _getAbsolutePath(server);
  logger.log("[RESPONSE-BUILDING] GetRequest: Parsed full path from request: " + path);


}

/*
 * Private methods
 */
std::string GetRequest::_getAbsolutePath(const Server& server) {
  const std::string uri       = getUri();
  const std::string path      = server.getRoot(uri);
  const std::string full_path = path.substr(0, path.size() - 1) + uri;
  return full_path;
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

/*
 * Checks if a file exists
 * @param path The path to the file to check
 * @return True if the file exists, false otherwise
 */
bool GetRequest::_fileExists(const std::string& path) {
  Logger&     logger = Logger::getInstance();

  struct stat buffer;  // stat struct to store the file info
  if (stat(path.c_str(), &buffer) == -1) {
    logger.log("_fileExists -> File doesn't exist (" + path + ")");
    return false;
  }
  if (buffer.st_mode & S_IFDIR) {
    logger.log("_fileExists -> File is a directory (" + path + ")");
    return false;
  }
  if (buffer.st_mode & S_IFREG) {
    logger.log("_fileExists -> File is a regular file (" + path + ")");
    return true;
  }
  logger.log("_fileExists -> File is something else (" + path + ")");
  logger.error("WARNING: logic probably not implemented yet");
  return false;
}
