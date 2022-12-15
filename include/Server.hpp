#pragma once
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/ioctl.h>
#include <sys/socket.h>
#include <unistd.h>

#include <General.hpp>

#include <HttpRequest.hpp>
#include <HttpResponse.hpp>
#include <CGI.hpp>
#include <Logger.hpp>
#include <cstdlib>
#include <cstring>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <climits>

#define MAX_PORT 65535

/* Default values */
#define DEFAULT_ERROR_STATUS 404
#define DEFAULT_ERROR_PATH "error.html"

#define DEFAULT_HOST_STATUS 420
#define DEFAULT_HOST_PATH "index.html"

#define DEFAULT_MAX_BODY 1000000

#define DEFAULT_HOST_STATUS 420
#define DEFAULT_HOST_PATH "index.html"

#define DEFAULT_MAX_BODY 1000000

#define DEFAULT_HOST "0.0.0.0"
#define DEFAULT_PORT 80

#define ROOT_FOLDER "root/"
/* End of default values */

/* status codes for processing requests */
#define INTERNAL_SERVER_ERROR 500
#define OK 200
#define BAD_REQUEST 400
/* End of status codes */

struct Route
{
  Route(std::string const &name) : route(name), rootDirectory(ROOT_FOLDER), defaultFile("index.html"), autoIndex(false)
  {
    allowedMethods[GET] = true;
    allowedMethods[POST] = true;
    allowedMethods[DELETE] = true;
  }
  // Default route constructor
  Route() : route("/"), rootDirectory(ROOT_FOLDER), cgiRoot(ROOT_FOLDER), defaultFile("index.html"), httpRedirection("")
  {
    allowedMethods[GET] = true;
    allowedMethods[POST] = true;
    allowedMethods[DELETE] = true;
  }
  // Vector for bonus
  std::string route;
  std::string rootDirectory;
  std::string cgiRoot;
  std::string defaultFile;
  std::map<EHttpMethods, bool> allowedMethods;
  std::vector<std::string> indexFiles;
  std::string httpRedirection;
  bool autoIndex;
};

struct PageData
{
  PageData(int const &statusCode, std::string const &filePath) : statusCode(statusCode), filePath(filePath) {}

  int statusCode;
  std::string filePath;
};

class Server
{
public:
  Server();
  ~Server();

  //  Getters
  std::string getServerName() const;
  std::vector<Route> getRoutes() const;
  std::string getHost() const;
  PageData getErrorPage() const;
  int getMaxBody() const;
  int getPort() const;

  // Setters
  int setPort(int const &value);
  int setHost(std::string const &host);
  int setErrorPage(int const &statusCode, std::string const &filePath);
  void setServerName(std::string const &value);
  int setMaxBody(double const &value);
  void addRoute(Route const &route);

private:
  int _port;
  std::string _host;
  int _maxBodySize;
  std::string _serverName;
  std::vector<Route> _routes;

  PageData _defaultErrorPage;
};
