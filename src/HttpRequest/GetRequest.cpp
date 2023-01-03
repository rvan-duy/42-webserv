#include "HttpRequest.hpp"
#include "Logger.hpp"

/* CONSTRUCTORS / DESTRUCTORS */

GetRequest::GetRequest() : HttpRequest() {}

GetRequest::GetRequest(HttpHeaderData const &data) : HttpRequest(data) {}

GetRequest::GetRequest(const GetRequest &ref) : HttpRequest(ref) {}

GetRequest &GetRequest::operator=(const GetRequest &ref) {
  if (this != &ref) {
    HttpRequest::operator=(ref);
  }
  return *this;
}

GetRequest::~GetRequest() {}

/* PUBLIC METHOD */

HttpResponse GetRequest::executeRequest(const Server &server) {
  const Route      &routeOfResponse = server.getRoute(_uri);
  const std::string path            = constructFullPath(routeOfResponse.rootDirectory, _uri);
  const FileType    fileType        = _getFileType(path);

  if (isMethodAllowed(routeOfResponse.allowedMethods, _method) == false) {
    return HttpResponse(HTTPStatusCode::METHOD_NOT_ALLOWED);
  }

  // TODO autoindex
  // dubbel check redirection

  switch (fileType) {
    case FileType::NOT_FOUND: {
      return _createRedirectionResponse(routeOfResponse);
    }
    case FileType::PYTHON_SCRIPT: {
      return createCGIResponse(routeOfResponse, path);
    }
    case FileType::DIRECTORY: {
      return _createDirectoryResponse(routeOfResponse, path);
    }
    case FileType::FILE: {
      return _createFileResponse(routeOfResponse, path);
    }
    default: {
      return _errorResponseWithHtml(HTTPStatusCode::INTERNAL_SERVER_ERROR, routeOfResponse);
    }
  }
}

/* PRIVATE METHODS */

HttpResponse GetRequest::_createRedirectionResponse(const Route &route) const {
  HttpResponse redirection;

  Logger::getInstance().log("Creating redirection response", VERBOSE);
  if (redirection.buildRedirection(route)) {
    return (redirection);
  } else {
    Logger::getInstance().log("Failed to build redirection response, returning 404", VERBOSE);
    return _errorResponseWithHtml(HTTPStatusCode::NOT_FOUND, route);
  }
}

HttpResponse GetRequest::_createDirectoryResponse(const Route &route, const std::string &path) const {
  if (_isTypeAccepted() == false) {
    return _errorResponseWithHtml(HTTPStatusCode::NOT_ACCEPTABLE, path);
  }
  std::vector<std::string> possiblePaths = _constructPossiblePaths(path, route.indexFiles);
  for (std::vector<std::string>::const_iterator it = possiblePaths.begin(); it != possiblePaths.end(); ++it) {
    if (_getFileType(*it) == FileType::FILE) {
      return _responseWithFile(*it, HTTPStatusCode::OK);
    }
  }
  return _errorResponseWithHtml(HTTPStatusCode::NOT_FOUND, route);
}

HttpResponse GetRequest::_createFileResponse(const Route &route, const std::string &path) const {
  if (_isTypeAccepted() == false) {
    return _errorResponseWithHtml(HTTPStatusCode::NOT_ACCEPTABLE, route);
  }
  return _responseWithFile(path, HTTPStatusCode::OK);
}

std::vector<std::string> GetRequest::_constructPossiblePaths(const std::string              &path,
                                                             const std::vector<std::string> &indexFiles) const {
  std::vector<std::string> possiblePaths;

  for (std::vector<std::string>::const_iterator it = indexFiles.begin(); it != indexFiles.end(); ++it) {
    possiblePaths.push_back(path + "/" + *it);
  }

  return possiblePaths;
}

bool GetRequest::_isTypeAccepted() const {
  const std::array<std::string, 4> typesToAccept = {"text/html", "text/css", "application/javascript", "image/jpg"};
  const std::string                acceptHeader  = getHeader("Accept");
  std::vector<std::string>         acceptedTypes;

  if (acceptHeader.empty() || acceptHeader.find("*/*") != std::string::npos) {
    return true;
  } else {
    // TODO: replace with splitheader function
    std::size_t pos  = 0;
    std::size_t prev = 0;
    while ((pos = acceptHeader.find(',', prev)) != std::string::npos) {
      acceptedTypes.push_back(acceptHeader.substr(prev, pos - prev));
      prev = pos + 1;
    }
    acceptedTypes.push_back(acceptHeader.substr(prev));
  }

  for (size_t i = 0; i < typesToAccept.size(); i++) {
    if (std::find(acceptedTypes.begin(), acceptedTypes.end(), typesToAccept[i]) != acceptedTypes.end()) {
      return true;
    }
  }
  return false;
}

HttpResponse GetRequest::_errorResponseWithHtml(HTTPStatusCode statusCode, Route const &route) const {
  std::string pathToErrorFile = _getErrorPage(route, statusCode);
  return _responseWithFile(pathToErrorFile, statusCode);
}

#define DEFAULT_ERROR_PAGE "root/error_pages/404/index.html"
std::string GetRequest::_getErrorPage(const Route &route, HTTPStatusCode errorCode) const {
  if (route.errorPages.find(errorCode) != route.errorPages.end()) {
    std::string page =
        route.rootDirectory + "/" + route.errorPages.at(errorCode);
    if (_getFileType(page) == FileType::FILE) {
      return page;
    }
  }
  return std::string(DEFAULT_ERROR_PAGE);
}

HttpResponse GetRequest::_errorResponse(HTTPStatusCode const &statusCode,
                                        Route const &route) const {
  std::string pathToErrorFile = _getErrorPage(route, statusCode);
  return _responseWithFile(pathToErrorFile, statusCode);
}
