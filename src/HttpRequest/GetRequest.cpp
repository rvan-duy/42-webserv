#include "HttpRequest.hpp"

GetRequest::GetRequest(std::string &msg) : HttpRequest(msg) {}

GetRequest::GetRequest(const GetRequest &ref) : HttpRequest(ref) {}

GetRequest::~GetRequest() {}

int GetRequest::executeRequest(Server &server)
{
  (void)server;
  return 0;
}

/**
 * Define a directory or a file from where the file should be searched (for
example, if url /kapouet is rooted to /tmp/www, url /kapouet/pouic/toto/pouet is /tmp/www/pouic/toto/pouet).
*/
// TODO: Make route getting great again
static Route getCorrectRoute(Server const &server, std::string _uri)
{
  std::vector<Route> routes = server.getRoutes();

  for (std::vector<Route>::iterator it = routes.begin(); it != routes.end(); it++)
  {
    if (it->route == _uri)
    {
      return *it;
    }
  }
  // TODO: If no route matches -> return default route
  return Route();
}

HttpResponse GetRequest::constructResponse(Server &server, std::string &index)
{
  Logger &logger = Logger::getInstance();
  logger.log("Response type: GET");
  HttpResponse response;
  (void)index;

  Route route = getCorrectRoute(server, _uri);

  // if (!_fileExists(path))
  // {
  //   logger.log("File doesn't exist");

  //   const std::string index_path = path + index;
  //   logger.log("Looking for index path: " + index_path);
  //   if (!_fileExists(index_path))
  //   {
  //     logger.log("Index file doesn't exist");

  //     response._setResponse("root/404/index.html", 404, "Not Found", getVersion()); // very hardcoded lol
  //     return response;
  //   }
  //   else
  //   {
  //     logger.log("Index file exists");
  //     path = index_path;
  //   }
  // }
  response._setResponse("hi", 200, "OK", getVersion());
  return response;
}
