#include "HttpRequest.hpp"

#define DEFAULT_ERROR_PAGE "root/404/error_pages/index.html"

GetRequest::GetRequest(std::string& msg) : HttpRequest(msg) {}

GetRequest::GetRequest(const GetRequest& ref) : HttpRequest(ref) {}

GetRequest::~GetRequest() {}

int GetRequest::executeRequest(const Server& server) {
  (void)server;
  return 0;
}

HttpResponse GetRequest::constructResponse(const Server& server) {
  const Route&                   routeOfResponse = server.getRoute(HttpRequest::_uri);
  const std::vector<std::string> accepted_types  = _getAcceptedTypesFromHeader();
  const std::string              path            = _constructPath(routeOfResponse);
  HttpResponse                   response;

  switch (_fileExists(path)) {
    case IS_DIR: {
      if (std::find(accepted_types.begin(), accepted_types.end(), "text/html") == accepted_types.end()) {
        response._setResponse(_getErrorPageIndex(routeOfResponse, 406), 406, "Not Acceptable", getVersion());
        return response;
      }
      std::vector<std::string> possible_paths = _getPossiblePaths(path, routeOfResponse.indexFiles);
      for (std::vector<std::string>::const_iterator it = possible_paths.begin(); it != possible_paths.end(); ++it) {
        if (_fileExists(*it) == IS_REG_FILE) {
          response._setResponse(*it, 200, "OK", getVersion());
          return response;
        }
      }
      response._setResponse(_getErrorPageIndex(routeOfResponse, 404), 404, "Not Found", getVersion());
      return response;
    }
    case IS_REG_FILE: {
      response._setResponse(path, 200, "OK", getVersion());
      return response;
    }
    case IS_UNKNOWN: {
      response._setResponse(_getErrorPageIndex(routeOfResponse, 404), 404, "Not Found", getVersion());
      return response;
    }
    default: {
      response._setResponse(_getErrorPageIndex(routeOfResponse, 500), 500, "Internal Server Error", getVersion());
      return response;
    }
  }
}

/*
 * Private methods
 */

std::string GetRequest::_getErrorPageIndex(const Route& route, int errorCode) const {
  std::map<int, std::string>::const_iterator it = route.errorPages.find(errorCode);
  if (it != route.errorPages.end() && _fileExists(route.rootDirectory + it->second) == IS_REG_FILE) {
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

std::string GetRequest::_constructPath(const Route& route) {
  const std::string uri       = getUri();
  const std::string path      = route.rootDirectory;
  const std::string full_path = path.substr(0, path.size() - 1) + uri;
  Logger::getInstance().log("[RESPONSE-BUILDING] GetRequest: _constructPath -> " + full_path);
  return full_path;
}

int GetRequest::_fileExists(const std::string& path) const {
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
