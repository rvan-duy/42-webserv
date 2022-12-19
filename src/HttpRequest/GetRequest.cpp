#include <HttpRequest.hpp>

#define DEFAULT_ERROR_PAGE "root/error_pages/404/index.html"

GetRequest::GetRequest(HttpHeaderData const &data) : HttpRequest(data) {}

GetRequest::GetRequest(const GetRequest &ref) : HttpRequest(ref) {}

GetRequest::~GetRequest() {}

HTTPStatusCode GetRequest::executeRequest(const Server &server) {
  (void)server;
  return HTTPStatusCode::OK;
}

HttpResponse GetRequest::constructResponse(const Server &server) {
  const Route       routeOfResponse = server.getRoute(HttpRequest::_uri);
  const std::string path            = _constructPath(routeOfResponse.rootDirectory);
  HttpResponse      response;

  if (_isMethodAllowed(routeOfResponse.allowedMethods) == false) {
    return _createResponseObject(path, HTTPStatusCode::METHOD_NOT_ALLOWED, routeOfResponse);
  }

  switch (_fileExists(path)) {
    case FileType::IS_DIR: {
      if (_typeIsAccepted() == false) {
        return _createResponseObject(path, HTTPStatusCode::NOT_ACCEPTABLE, routeOfResponse);
      }
      std::vector<std::string> possible_paths = _getPossiblePaths(path, routeOfResponse.indexFiles);
      for (std::vector<std::string>::const_iterator it = possible_paths.begin(); it != possible_paths.end(); ++it) {
        if (_fileExists(*it) == FileType::IS_REG_FILE) {
          return _createResponseObject(*it, HTTPStatusCode::OK, routeOfResponse);
        }
      }
      return _createResponseObject(path, HTTPStatusCode::NOT_FOUND, routeOfResponse);
    }
    case FileType::IS_REG_FILE: {
      if (_typeIsAccepted() == false) {
        return _createResponseObject(path, HTTPStatusCode::NOT_ACCEPTABLE, routeOfResponse);
      }
      return _createResponseObject(path, HTTPStatusCode::OK, routeOfResponse);
    }
    case FileType::IS_UNKNOWN: {
      return _createResponseObject(path, HTTPStatusCode::NOT_FOUND, routeOfResponse);
    }
    default: {
      return _createResponseObject(path, HTTPStatusCode::INTERNAL_SERVER_ERROR, routeOfResponse);
    }
  }
}

/*
 * Private methods
 */

HttpResponse GetRequest::_createResponseObject(const std::string &path, HTTPStatusCode statusCode, const Route &route) const {
  HttpResponse response;

  if (statusCode == HTTPStatusCode::OK) {
    response._setResponse(path, statusCode, getMessageByStatusCode(statusCode), getVersion());
    return response;
  } else {
    response._setResponse(_getErrorPageIndex(route, statusCode), statusCode, getMessageByStatusCode(statusCode), getVersion());
    return response;
  }
}

bool GetRequest::_typeIsAccepted() const {
  // TYPES_TO_ACCEPT is a vector of strings that contains all the types that the server can accept
  const std::vector<std::string> TYPES_TO_ACCEPT = {"text/html", "text/css", "application/javascript"};
  const std::string              ACCEPT_HEADER   = getHeader("Accept");
  std::vector<std::string>       acceptedTypes;

  if (ACCEPT_HEADER.empty() || ACCEPT_HEADER == "*/*") {
    return true;
  } else {
    std::size_t pos  = 0;
    std::size_t prev = 0;
    while ((pos = ACCEPT_HEADER.find(',', prev)) != std::string::npos) {
      acceptedTypes.push_back(ACCEPT_HEADER.substr(prev, pos - prev));
      prev = pos + 1;
    }
    acceptedTypes.push_back(ACCEPT_HEADER.substr(prev));
  }

  for (std::vector<std::string>::const_iterator it = acceptedTypes.begin(); it != acceptedTypes.end(); ++it) {
    for (std::vector<std::string>::const_iterator it2 = TYPES_TO_ACCEPT.begin(); it2 != TYPES_TO_ACCEPT.end(); ++it2) {
      if (*it == *it2) {
        return true;
      }
    }
  }

  return false;
}

std::string GetRequest::_getErrorPageIndex(const Route &route, HTTPStatusCode errorCode) const {
  std::map<HTTPStatusCode, std::string>::const_iterator it = route.errorPages.find(errorCode);
  if (it != route.errorPages.end() && _fileExists(route.rootDirectory + it->second) == FileType::IS_REG_FILE) {
    Logger::getInstance().log("GetRequest::_getErrorPageIndex: " + route.rootDirectory + it->second);
    return route.rootDirectory + it->second;
  }
  Logger::getInstance().log("GetRequest::_getErrorPageIndex (DEFAULT): " + std::string(DEFAULT_ERROR_PAGE));
  return std::string(DEFAULT_ERROR_PAGE);
}

std::vector<std::string> GetRequest::_getPossiblePaths(const std::string              &path,
                                                       const std::vector<std::string> &index_files) {
  std::vector<std::string> possible_paths;

  for (std::vector<std::string>::const_iterator it = index_files.begin(); it != index_files.end(); ++it) {
    possible_paths.push_back(path + "/" + *it);
  }

  return possible_paths;
}

std::string GetRequest::_constructPath(const std::string &root) const {
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

FileType GetRequest::_fileExists(const std::string &path) const {
  Logger     &logger = Logger::getInstance();

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
