#include <HttpRequest.hpp>

GetRequest::GetRequest(HttpHeaderData const &data) : HttpRequest(data) {}

GetRequest::GetRequest(const GetRequest &ref) : HttpRequest(ref) {}

GetRequest::~GetRequest() {}

HTTPStatusCode GetRequest::executeRequest(const Server &server) {
  (void)server;
  return HTTPStatusCode::OK;
}

// TODO: add redirection support, with return ? 301 and 302
// auto index
// - if auto index is on and the path is a directory, look for index files and
// return it
// - if auto index is off and the path is a directory, return 403
HttpResponse GetRequest::constructResponse(const Server &server) {
  Route const &routeOfResponse = server.getRoute(HttpRequest::_uri);
  const std::string path = _constructPath(routeOfResponse.rootDirectory);

  if (_isMethodAllowed(routeOfResponse.allowedMethods) == false) {
    return HttpResponse(HTTPStatusCode::METHOD_NOT_ALLOWED);
  }

  if (_isCgiRequest(path)) {
    return _handleCgiRequest(path, routeOfResponse);
  }
  return _handleFileRequest(path, routeOfResponse);
}

HttpResponse GetRequest::_handleCgiRequest(std::string const &path,
                                           Route const &route) const {
  std::map<std::string, std::string> headers;
  std::string body;

  if (!route.cgiEnabled) {
    return _errorResponseWithFile(HTTPStatusCode::METHOD_NOT_ALLOWED, route);
  }
  HTTPStatusCode status = CGI::executeFile(&body, &headers, path, "");
  if (status != HTTPStatusCode::OK) {
    Logger::getInstance().error(getMessageByStatusCode(status));
    return _errorResponseWithFile(status, route);
  }
  return _responseWithBody(headers, body);
}

HttpResponse GetRequest::_handleFileRequest(std::string const &path,
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
