#include "HttpRequest.hpp"

GetRequest::GetRequest(std::string& msg) : HttpRequest(msg) {}

GetRequest::GetRequest(const GetRequest& ref) : HttpRequest(ref) {}

GetRequest::~GetRequest() {}

int GetRequest::executeRequest(const Server& server) {
  (void)server;
  return 0;
}



HttpResponse GetRequest::constructResponse(const Server& server) {
  Logger&                  logger         = Logger::getInstance();
  std::vector<std::string> accepted_types = _getAcceptedTypesFromHeader();
  std::string              path           = _getPath(server);
  HttpResponse             response;

  // TODO: Check if file permissions are allowed
  // TODO: Replace the switch with a std::map
  switch(_fileExists(path)) {
    case IS_DIR:
      if (std::find(accepted_types.begin(), accepted_types.end(), "text/html") == accepted_types.end()) {
        // TODO make a 406 error page
        response._setResponse("root/406/index.html", 406, "Not Acceptable", getVersion());
        return response;
      }
      path += "/index.html";  // TODO: make this a config option
      break;
    case IS_REG_FILE:
      break;
    case IS_UNKNOWN:
      response._setResponse("root/404/index.html", 404, "Not Found", getVersion());
      return response;
    default:
      response._setResponse("root/500/index.html", 500, "Internal Server Error", getVersion());
      return response;
  }

  if (_fileExists(path) == IS_UNKNOWN) {
    response._setResponse("root/404/index.html", 404, "Not Found", getVersion());
    return response;
  }

  response._setResponse(path, 200, "OK", getVersion());
  return response;
}

/*
 * Private methods
 */

std::vector<std::string> GetRequest::_getAcceptedTypesFromHeader() {
  std::string              accept = getHeader("Accept");
  std::vector<std::string> accepted_types;
  std::vector<std::string> all_types = {"text/html", "text/css", "application/javascript"};

  if (accept.find("*/*") != std::string::npos) {
    return all_types;
  }

  for (std::vector<std::string>::iterator it = all_types.begin(); it != all_types.end(); ++it) {
    if (it->find(accept) != std::string::npos) {
      accepted_types.push_back(*it);
    }
  }

  return accepted_types;
}

std::string GetRequest::_getPath(const Server& server) {
  const std::string uri       = getUri();
  const std::string path      = server.getRoot(uri);
  const std::string full_path = path.substr(0, path.size() - 1) + uri;
  Logger::getInstance().log("[RESPONSE-BUILDING] GetRequest: _getPath -> " + full_path);
  return full_path;
}

int GetRequest::_fileExists(const std::string& path) {
  Logger&     logger = Logger::getInstance();

  struct stat buffer;
  if (stat(path.c_str(), &buffer) == -1) {
    logger.log("[RESPONSE-BUILDING] GetRequest: _fileExists -> File doesn't exist (" + path + ")");
    return IS_UNKNOWN;
  }
  if (buffer.st_mode & S_IFDIR) {
    logger.log("[RESPONSE-BUILDING] GetRequest: _fileExists -> File is a directory (" + path + ")");
    return IS_DIR;
  }
  if (buffer.st_mode & S_IFREG) {
    logger.log("[RESPONSE-BUILDING] GetRequest: _fileExists -> File is a regular file (" + path + ")");
    return IS_REG_FILE;
  }
  logger.log("[RESPONSE-BUILDING] GetRequest: _fileExists -> File is something else (" + path + ")");
  logger.error("WARNING: logic probably not implemented yet");
  return IS_UNKNOWN;
}
