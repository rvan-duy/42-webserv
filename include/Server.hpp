#pragma once
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/ioctl.h>
#include <sys/socket.h>
#include <unistd.h>

#include <General.hpp>

#include <HttpRequest.hpp>
#include <HttpResponse.hpp>
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
#define DEFAULT_ERROR_STATUS 666
#define DEFAULT_ERROR_PATH "defaultErrorPage"

#define DEFAULT_HOST_STATUS 777
#define DEFAULT_HOST_PATH "default host path"

struct Route
{
  std::string route;
  std::map<EHttpMethods, bool> allowedMethods;
  std::string httpRedirection;
  std::string searchDirectory;
  std::string defaultFile;
  std::string cgiParam;
  std::string rootDirectory;
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
  std::vector<std::string> getServerName() const;
  std::vector<Route> getRoutes() const;
  PageData getHost() const;
  PageData getErrorPage() const;
  int getMaxBody() const;
  int getPort() const;
  int getFd() const;
  std::vector<int> &getConnectedClients();
  HttpRequest *getRequestByDiscriptor(int fd);

  // Setters
  int setHost(int const &statusCode, std::string const &filePath);
  int setErrorPage(int const &statusCode, std::string const &filePath);
  void setServerName(std::vector<std::string> const &value);
  int setPort(int const &value);
  int setMaxBody(double const &value);
  void addRoute(Route const &route);

  // Request generation
  HttpRequest *createRequest(std::string &msg);

private:
  int _port;
  int _maxBodySize;
  std::vector<std::string> _serverName;
  std::map<int, HttpRequest *> _requests;
  std::vector<Route> _routes;
  PageData _defaultErrorPage;
  PageData _host;
};
