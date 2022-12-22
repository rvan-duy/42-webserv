#include <HttpRequest.hpp>

#include "Logger.hpp"

/*
 *  Canonical
 */
HttpRequest::HttpRequest(HttpHeaderData const &data)
    : HttpMessage(data.httpVersion, data.headers, data.body),
      _method(data.method),
      _uri(data.url),
      _chunked(data.chunked) {}

HttpRequest::HttpRequest() : _chunked(false) {}

HttpRequest::HttpRequest(const HttpRequest &obj) : HttpMessage(obj) {
  _method = obj._method;
  _uri = obj._uri;
  _chunked = obj._chunked;
}

HttpRequest::~HttpRequest() {}

/*
 * Protected methods
 */
HttpResponse HttpRequest::_handleCgiRequest(std::string const &path,
                                            Route const &route) const {
  std::map<std::string, std::string> headers;
  std::string body;

  if (!route.cgiEnabled) {
    return _errorResponseWithFile(HTTPStatusCode::METHOD_NOT_ALLOWED, route);
  }
  HTTPStatusCode status = CGI::executeFile(&body, &headers, path, _body);

  if (status != HTTPStatusCode::OK) {
    Logger::getInstance().error("Executing cgi: " +
                                getMessageByStatusCode(status));
    return _errorResponseWithFile(status, route);
  }
  return _responseWithBody(headers, body);
}

HttpResponse HttpRequest::_handleFileRequest(std::string const &path,
                                             Route const &route) const {
  switch (_getFileType(path)) {
    case FileType::IS_DIR: {
      if (_isTypeAccepted() == false) {
        return _errorResponseWithFile(HTTPStatusCode::NOT_ACCEPTABLE, route);
      }
      std::vector<std::string> possiblePaths =
          _getPossiblePaths(path, route.indexFiles);
      for (std::vector<std::string>::const_iterator it = possiblePaths.begin();
           it != possiblePaths.end(); ++it) {
        if (_getFileType(*it) == FileType::IS_REG_FILE) {
          return _responseWithFile(*it, HTTPStatusCode::OK);
        }
      }
      return HttpResponse(HTTPStatusCode::NOT_FOUND);
    }
    case FileType::IS_REG_FILE: {
      if (_isTypeAccepted() == false) {
        return _errorResponseWithFile(HTTPStatusCode::NOT_ACCEPTABLE, route);
      }
      return _responseWithFile(path, HTTPStatusCode::OK);
    }
    case FileType::IS_UNKNOWN: {
      return _errorResponseWithFile(HTTPStatusCode::NOT_FOUND, route);
    }
    default: {
      return _errorResponseWithFile(HTTPStatusCode::INTERNAL_SERVER_ERROR,
                                    route);
    }
  }
}

bool HttpRequest::_isMethodAllowed(
    const std::map<EHttpMethods, bool> allowedMethods) const {
  EHttpMethods currentMethod = _method;

  for (std::map<EHttpMethods, bool>::const_iterator it = allowedMethods.begin();
       it != allowedMethods.end(); it++) {
    if (it->first == currentMethod) {
      return it->second;
    }
  }
  return false;
}

bool HttpRequest::_isCgiRequest(std::string path) const {
  if (path.substr(path.length() - 3) == ".py") {
    return true;
  }
  return false;
}

static std::string getFileSize(std::ifstream &file) {
  std::stringstream ss;

  file.seekg(0, std::ios::end);
  ss << file.tellg();
  file.seekg(0, std::ios::beg);
  return ss.str();
}

static std::string fileToStr(std::ifstream &file) {
  file.seekg(0, std::ios::beg);

  std::string body((std::istreambuf_iterator<char>(file)),
                   std::istreambuf_iterator<char>());
  return body;
}

/*
 * Get the file type of a file
 * @param path The path to the file to check
 * @return The content type of the file
 */
static std::string getContentType(const std::string &path) {
  std::map<std::string, std::string> file_types;

  file_types["html"] = "text/html";
  file_types["css"] = "text/css";
  file_types["js"] = "application/javascript";
  file_types["png"] = "image/png";

  std::string file_extension = path.substr(path.find_last_of(".") + 1);
  if (file_types.find(file_extension) != file_types.end()) {
    return file_types[file_extension];
  }
  return "text/plain";
}

#define DEFAULT_ERROR_PAGE "root/error_pages/404/index.html"
std::string HttpRequest::_getErrorPageIndex(const Route &route,
                                            HTTPStatusCode errorCode) const {
  if (route.errorPages.find(errorCode) != route.errorPages.end()) {
    std::string page = route.errorPages.at(errorCode);
    if (_getFileType(page) == FileType::IS_REG_FILE) {
      return route.rootDirectory + page;
    }
  }
  // TODO: fix hardcode
  return std::string(DEFAULT_ERROR_PAGE);
}

HttpResponse HttpRequest::_errorResponseWithFile(
    HTTPStatusCode const &statusCode, Route const &route) const {
  std::string pathToErrorFile = _getErrorPageIndex(route, statusCode);
  return _responseWithFile(pathToErrorFile, statusCode);
}

HttpResponse HttpRequest::_responseWithFile(std::string const &path,
                                            HTTPStatusCode statusCode) const {
  std::ifstream file(path.c_str());
  if (!file.is_open()) {
    return HttpResponse(HTTPStatusCode::INTERNAL_SERVER_ERROR);
  }
  HttpResponse response(statusCode);
  std::string test = fileToStr(file);
  response.setBody(fileToStr(file));
  response.setHeader("Content-Length", getFileSize(file));
  response.setHeader("Content-Type", getContentType(path));
  return response;
}

HttpResponse HttpRequest::_responseWithBody(
    std::map<std::string, std::string> headers, std::string body) const {
  HttpResponse response(HTTPStatusCode::OK);
  response.setBody(body);
  Logger::getInstance().debug(body);
  for (std::map<std::string, std::string>::const_iterator it = headers.begin();
       it != headers.end(); ++it) {
    Logger::getInstance().debug(it->first);
    response.setHeader(it->first, it->second);
  }
  return response;
}

FileType HttpRequest::_getFileType(const std::string &path) const {
  Logger &logger = Logger::getInstance();

  struct stat buffer;

  if (stat(path.c_str(), &buffer) == -1) {
    logger.log(
        "[RESPONSE-BUILDING] GetRequest: _fileExists -> File doesn't exist "
        "(" +
            path + ")",
        VERBOSE);
    return FileType::IS_UNKNOWN;
  }
  if (buffer.st_mode & S_IFDIR) {
    logger.log(
        "[RESPONSE-BUILDING] GetRequest: _fileExists -> File is a directory "
        "(" +
            path + ")",
        VERBOSE);
    return FileType::IS_DIR;
  }
  if (buffer.st_mode & S_IFREG) {
    logger.log(
        "[RESPONSE-BUILDING] GetRequest: _fileExists -> File is a regular "
        "file "
        "(" +
            path + ")",
        VERBOSE);
    return FileType::IS_REG_FILE;
  }
  logger.log(
      "[RESPONSE-BUILDING] GetRequest: _fileExists -> File is something else "
      "(" +
          path + ")",
      VERBOSE);
  logger.error("WARNING: logic probably not implemented yet");
  return FileType::IS_UNKNOWN;
}

std::vector<std::string> HttpRequest::_getPossiblePaths(
    const std::string &path, const std::vector<std::string> &indexFiles) const {
  std::vector<std::string> possiblePaths;

  for (std::vector<std::string>::const_iterator it = indexFiles.begin();
       it != indexFiles.end(); ++it) {
    possiblePaths.push_back(path + "/" + *it);
  }

  return possiblePaths;
}

std::string HttpRequest::_constructPath(const std::string &root) const {
  std::string fullPath;

  // eg: root = "root/", _uri = "/index.html"
  // => full_path = "root/index.html"
  if (!root.empty()) {
    return root + _uri;
  } else {
    return _uri;
  }
}

bool HttpRequest::_isTypeAccepted() const {
  // All types the server accepts
  const std::array<std::string, 4> typesToAccept = {
      "text/html", "text/css", "application/javascript", "image/jpg"};
  const std::string acceptHeader = getHeader("Accept");
  std::vector<std::string> acceptedTypes;

  if (acceptHeader.empty() || acceptHeader.find("*/*") != std::string::npos) {
    return true;
  } else {
    std::size_t pos = 0;
    std::size_t prev = 0;
    while ((pos = acceptHeader.find(',', prev)) != std::string::npos) {
      acceptedTypes.push_back(acceptHeader.substr(prev, pos - prev));
      prev = pos + 1;
    }
    acceptedTypes.push_back(acceptHeader.substr(prev));
  }

  for (size_t i = 0; i < typesToAccept.size(); i++) {
    if (std::find(acceptedTypes.begin(), acceptedTypes.end(),
                  typesToAccept[i]) != acceptedTypes.end()) {
      return true;
    }
  }

  return false;
}
