#include "HttpRequest.hpp"

GetRequest::GetRequest(HttpHeaderData const &data) : HttpRequest(data) {}

GetRequest::GetRequest(const GetRequest &ref) : HttpRequest(ref) {}

GetRequest::~GetRequest() {}

HTTPStatusCode GetRequest::executeRequest(Server &server)
{
  (void)server;
  return HTTPStatusCode::OK;
}

HttpResponse GetRequest::constructResponse(Server &server, std::string &index)
{
  Logger &logger = Logger::getInstance();
  logger.log("Response type: GET");
  (void)server;
  HttpResponse response;

  std::string path = "root" + getUrl(); // future get root from server class
  logger.log("Path: " + path);

  if (!_fileExists(path))
  {
    logger.log("File doesn't exist");

    const std::string index_path = path + index;
    logger.log("Looking for index path: " + index_path);
    if (!_fileExists(index_path))
    {
      logger.log("Index file doesn't exist");

      response._setResponse("root/404/index.html", HTTPStatusCode::NOT_FOUND, "Not Found", getVersion()); // very hardcoded lol
      return response;
    }
    else
    {
      logger.log("Index file exists");
      path = index_path;
    }
  }
  response._setResponse(path, HTTPStatusCode::OK, "OK", getVersion());
  return response;
}
