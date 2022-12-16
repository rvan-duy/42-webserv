#include "HttpRequest.hpp"

#define DEFAULT_ERROR_PAGE "root/error_pages/404/index.html"

GetRequest::GetRequest(std::string& msg) : HttpRequest(msg) {}

GetRequest::GetRequest(const GetRequest& ref) : HttpRequest(ref) {}

GetRequest::~GetRequest() {}

int GetRequest::executeRequest(const Server& server) {
  (void)server;
  return 0;
}

// no permissions check

HttpResponse GetRequest::constructResponse(const Server& server) {
  const Route&                   routeOfResponse = server.getRoute(HttpRequest::_uri);
  const std::vector<std::string> acceptedTypes   = _getAcceptedTypesFromHeader();
  const std::string              path            = _constructPath(routeOfResponse.rootDirectory);
  HttpResponse                   response;

  if (_isMethodAllowed(routeOfResponse.allowedMethods) == false) {
    response._setResponse(_getErrorPageIndex(routeOfResponse, HTTPStatusCode::METHOD_NOT_ALLOWED),
                          HTTPStatusCode::METHOD_NOT_ALLOWED, "Method Not Allowed", _version);
    return response;
  }

  switch (_fileExists(path)) {
    case FileType::IS_DIR: {
      if (std::find(acceptedTypes.begin(), acceptedTypes.end(), "text/html") == acceptedTypes.end()) {
        response._setResponse(_getErrorPageIndex(routeOfResponse, HTTPStatusCode::NOT_ACCEPTABLE),
                              HTTPStatusCode::NOT_ACCEPTABLE, "Not Acceptable", getVersion());
        return response;
      }
      std::vector<std::string> possible_paths = _getPossiblePaths(path, routeOfResponse.indexFiles);
      for (std::vector<std::string>::const_iterator it = possible_paths.begin(); it != possible_paths.end(); ++it) {
        if (_fileExists(*it) == FileType::IS_REG_FILE) {
          response._setResponse(*it, HTTPStatusCode::OK, "OK", getVersion());
          return response;
        }
      }
      response._setResponse(_getErrorPageIndex(routeOfResponse, HTTPStatusCode::NOT_FOUND), HTTPStatusCode::NOT_FOUND,
                            "Not Found", getVersion());
      return response;
    }
    case FileType::IS_REG_FILE: {
      if (acceptedTypes.empty()) {
        response._setResponse(_getErrorPageIndex(routeOfResponse, HTTPStatusCode::NOT_ACCEPTABLE),
                              HTTPStatusCode::NOT_ACCEPTABLE, "Not Acceptable", getVersion());
      } else {
        response._setResponse(path, HTTPStatusCode::OK, "OK", getVersion());
      }
      return response;
    }
    case FileType::IS_UNKNOWN: {
      response._setResponse(_getErrorPageIndex(routeOfResponse, HTTPStatusCode::NOT_FOUND), HTTPStatusCode::NOT_FOUND,
                            "Not Found", getVersion());
      return response;
    }
    default: {
      response._setResponse(_getErrorPageIndex(routeOfResponse, HTTPStatusCode::INTERNAL_SERVER_ERROR),
                            HTTPStatusCode::INTERNAL_SERVER_ERROR, "Internal Server Error", getVersion());
      return response;
    }
  }
}

/*
 * Private methods
 */

std::string GetRequest::_getErrorPageIndex(const Route& route, HTTPStatusCode errorCode) const {
  std::map<HTTPStatusCode, std::string>::const_iterator it = route.errorPages.find(errorCode);
  if (it != route.errorPages.end() && _fileExists(route.rootDirectory + it->second) == FileType::IS_REG_FILE) {
    return route.rootDirectory + it->second;
  }
  return std::string(DEFAULT_ERROR_PAGE);
}

std::vector<std::string> GetRequest::_getPossiblePaths(const std::string&              path,
                                                       const std::vector<std::string>& index_files) {
  std::vector<std::string> possible_paths;

  for (std::vector<std::string>::const_iterator it = index_files.begin(); it != index_files.end(); ++it) {
    possible_paths.push_back(path + "/" + *it);
  }

  return possible_paths;
}

std::vector<std::string> GetRequest::_getAcceptedTypesFromHeader() {
  std::string              accept = getHeader("Accept");
  std::vector<std::string> acceptedTypes;
  std::vector<std::string> all_types = {"text/html", "text/css", "application/javascript"};

  if (accept.find("*/*") != std::string::npos) {
    return all_types;
  }

  for (std::vector<std::string>::iterator it = all_types.begin(); it != all_types.end(); ++it) {
    if (it->find(accept) != std::string::npos) {
      acceptedTypes.push_back(*it);
    }
  }

  return acceptedTypes;
}

std::string GetRequest::_constructPath(const std::string& root) const {
  std::string full_path;

  // eg: root = "root/", _uri = "/index.html"
  // => full_path = "root/index.html"
  if (!root.empty()) {
    full_path = root.substr(0, root.size() - 1) + _uri;
  } else {
    full_path = _uri;
  }

  Logger::getInstance().log("[RESPONSE-BUILDING] GetRequest: _constructPath -> " + full_path);
  return full_path;
}

FileType GetRequest::_fileExists(const std::string& path) const {
  Logger&     logger = Logger::getInstance();

  struct stat buffer;
  if (stat(path.c_str(), &buffer) == -1) {
    logger.log("[RESPONSE-BUILDING] GetRequest: _fileExists -> File doesn't exist (" + path + ")");
    return FileType::IS_UNKNOWN;
  }
  if (buffer.st_mode & S_IFDIR) {
    logger.log("[RESPONSE-BUILDING] GetRequest: _fileExists -> File is a directory (" + path + ")");
    return FileType::IS_DIR;
  }
  if (buffer.st_mode & S_IFREG) {
    logger.log("[RESPONSE-BUILDING] GetRequest: _fileExists -> File is a regular file (" + path + ")");
    return FileType::IS_REG_FILE;
  }
  logger.log("[RESPONSE-BUILDING] GetRequest: _fileExists -> File is something else (" + path + ")");
  logger.error("WARNING: logic probably not implemented yet");
  return FileType::IS_UNKNOWN;
}
