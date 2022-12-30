#include "HttpRequest.hpp"

#include "Logger.hpp"

/* CONSTRUCTORS / DESTRUCTORS */

HttpRequest::HttpRequest() : HttpMessage() {}

HttpRequest::HttpRequest(const HttpHeaderData &data) : HttpMessage(data.httpVersion, data.headers, data.body) {
  _method = data.method;
  _uri    = data.url;
}

HttpRequest::HttpRequest(const HttpRequest &ref) : HttpMessage(ref) {
  _method = ref._method;
  _uri    = ref._uri;
}

HttpRequest &HttpRequest::operator=(const HttpRequest &ref) {
  if (this != &ref) {
    HttpMessage::operator=(ref);
    _method = ref._method;
    _uri    = ref._uri;
  }
  return *this;
}

HttpRequest::~HttpRequest() {}

/* PUBLIC VIRTUAL METHOD */

HttpResponse HttpRequest::executeRequest(const Server &server) {
  const Route      &routeOfResponse = server.getRoute(_uri);
  const std::string path            = constructFullPath(routeOfResponse.rootDirectory, _uri);
  const FileType    fileType        = getFileType(path);

  if (isMethodAllowed(routeOfResponse.allowedMethods, _method) == false) {
    return HttpResponse(HTTPStatusCode::METHOD_NOT_ALLOWED);
  }

  switch (fileType) {
    case FileType::NOT_FOUND: {
      return HttpResponse(HTTPStatusCode::NOT_FOUND);
    }
    case FileType::PYTHON_SCRIPT: {
      return createCGIResponse(routeOfResponse, path);
    }
    case FileType::DIRECTORY: {
      return HttpResponse(HTTPStatusCode::FORBIDDEN);
    }
    case FileType::REGULAR_FILE: {
      return HttpResponse(HTTPStatusCode::FORBIDDEN);
    }
    default: {
      return HttpResponse(HTTPStatusCode::INTERNAL_SERVER_ERROR);
    }
  }
}

/* CHUNKED */

HttpRequest *HttpRequest::operator+(const HttpRequest &chunk) {
  if (chunk._statusCode != HTTPStatusCode::NOT_SET) {
    delete this;
    return new BadRequest(HTTPStatusCode::BAD_REQUEST);
  }
  _body += chunk._body;
  _headers.insert(std::begin(chunk._headers), std::end(chunk._headers));
  return this;
}

void HttpRequest::unChunkBody() {
  _body = unChunk(_body);
}

/**
 * Determines whether a given HTTP request uses chunked transfer encoding.
 */
bool HttpRequest::isFirstChunk() {
  return "chunked" == getHeader("Transfer-Encoding");
}

/* PROTECTED METHODS THAT ARE USED BY CHILD CLASSES */

bool HttpRequest::isMethodAllowed(const MethodMap allowedMethods, const EHttpMethods &method) const {
  for (MethodMap::const_iterator it = allowedMethods.begin(); it != allowedMethods.end(); it++) {
    if (it->first == method) {
      Logger::getInstance().log("Method allowed", VERBOSE);
      return true;
    }
  }
  Logger::getInstance().error("Method not allowed");
  return false;
}

FileType HttpRequest::getFileType(const std::string &path) const {
  struct stat buf;
  if (stat(path.c_str(), &buf) == -1) {
    Logger::getInstance().error("File not found");
    return FileType::NOT_FOUND;
  }
  if (S_ISDIR(buf.st_mode)) {
    Logger::getInstance().log("File is a directory", VERBOSE);
    return FileType::DIRECTORY;
  }
  if (S_ISREG(buf.st_mode)) {
    if (path.find(".py") != std::string::npos) {
      Logger::getInstance().log("File is a Python script", VERBOSE);
      return FileType::PYTHON_SCRIPT;
    }
    Logger::getInstance().log("File is a regular file", VERBOSE);
    return FileType::REGULAR_FILE;
  }
  Logger::getInstance().error("File is something else");
  return FileType::NOT_FOUND;
}

std::string HttpRequest::constructFullPath(const std::string &rootDirectory, const std::string &uri) const {
  std::string path = rootDirectory + uri;
  if (path[path.length() - 1] == '/') {
    path += "index.html";
  }
  Logger::getInstance().log("Constructed full path: " + path, VERBOSE);
  return path;
}

HttpResponse HttpRequest::createCGIResponse(const Route &route, const std::string &path) const {
  Logger                            &logger = Logger::getInstance();
  std::map<std::string, std::string> headers;
  std::string                        body;

  logger.log("starting cgi request", VERBOSE);
  if (!route.cgiEnabled) {
    logger.error("CGI not allowed on this route");
    return HttpResponse(HTTPStatusCode::METHOD_NOT_ALLOWED);
  }

  HTTPStatusCode status = CGI::executeFile(&body, &headers, path, _body);
  if (status != HTTPStatusCode::OK) {
    Logger::getInstance().error("Executing cgi: " + getMessageByStatusCode(status));
    return HttpResponse(status);
  }

  logger.log("CGI request successful", VERBOSE);
  return responseWithBody(headers, body);
}

HttpResponse HttpRequest::responseWithBody(HeaderMap headers, std::string body) const {
  HttpResponse response(HTTPStatusCode::OK);
  response.setBody(body);
  Logger::getInstance().debug(body);
  for (HeaderMap::const_iterator it = headers.begin(); it != headers.end(); ++it) {
    Logger::getInstance().debug(it->first);
    Logger::getInstance().debug(it->second);
    response.setHeader(it->first, it->second);
  }
  return response;
}

HttpResponse HttpRequest::responseWithFile(const std::string &path, HTTPStatusCode statusCode) const {
  std::ifstream file(path.c_str());
  if (!file.is_open()) {
    return HttpResponse(HTTPStatusCode::INTERNAL_SERVER_ERROR);
  }
  HttpResponse response(statusCode);
  std::string  test = fileToStr(file);
  response.setBody(fileToStr(file));
  response.setHeader("Content-Length", getFileSize(file));
  response.setHeader("Content-Type", getContentType(path));
  return response;
}

std::string HttpRequest::fileToStr(std::ifstream &file) const {
  file.seekg(0, std::ios::beg);

  std::string body((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
  return body;
}

std::string HttpRequest::getFileSize(std::ifstream &file) const {
  std::stringstream ss;

  file.seekg(0, std::ios::end);
  ss << file.tellg();
  file.seekg(0, std::ios::beg);
  return ss.str();
}

std::string HttpRequest::getContentType(const std::string &path) const {
  std::map<std::string, std::string> file_types;

  file_types["html"]         = "text/html";
  file_types["css"]          = "text/css";
  file_types["js"]           = "application/javascript";
  file_types["png"]          = "image/png";

  std::string file_extension = path.substr(path.find_last_of(".") + 1);
  if (file_types.find(file_extension) != file_types.end()) {
    return file_types[file_extension];
  }
  return "text/plain";
}