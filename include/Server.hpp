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

/****************************************************************************************************/
/* The Server Class (formerly known as Socket)                                                      */
/* - A socket is a mechanism that provides programs access to the network, it allows messages to be */
/*   sent and received between programs.                                                            */
/* - A socket is a combination of an IP address and a port number.                                  */
/* - A socket is a file descriptor, and is used to read and write data to the network.              */
/****************************************************************************************************/
/* A few steps are involved with sockets                                                            */
/* 1. Create a socket                                                                               */
/* 2. Bind the socket to an address and port number (also known as identifying the socket,          */
/*      giving it a name..)                                                                         */
/* 3. Wait for an incoming connection                                                               */
/* 4. Send and receive messages                                                                     */
/* 5. Close socket                                                                                  */
/****************************************************************************************************/

/*
In the configuration file, you should be able to:
• Choose the port and host of each ’server’.
• Setup the server_names or not.
• The first server for a host:port will be the default for this host:port (that means it will answer to all the requests
that don’t belong to an other server). • Setup default error pages. • Limit client body size. • Setup routes with one or
multiple of the following rules/configuration (routes wont be using regexp): ◦ Define a list of accepted HTTP methods
for the route. ◦ Define a HTTP redirection. ◦ Define a directory or a file from where the file should be searched (for
example, if url /kapouet is rooted to /tmp/www, url /kapouet/pouic/toto/pouet is /tmp/www/pouic/toto/pouet). ◦ Turn on
or off directory listing. ◦ Set a default file to answer if the request is a directory. ◦ Execute CGI based on certain
file extension (for example .php). ◦ Make the route able to accept uploaded files and configure where they should be
saved. ∗ Do you wonder what a CGI is? ∗ Because you won’t call the CGI directly, use the full path as PATH_INFO. ∗ Just
remember that, for chunked request, your server needs to unchunked it and the CGI will expect EOF as end of the body. ∗
Same things for the output of the CGI. If no content_length is returned from the CGI, EOF will mark the end of the
returned data. ∗ Your program should call the CGI with the file requested as first argument. ∗ The CGI should be run in
the correct directory for relative path file access. ∗ Your server should work with one CGI (php-CGI, Python, and so
forth). You must provide some configuration files and default basic files to test and demon- strate every feature works
during evaluation.
*/

#define MAX_PORT 65535

/* Default values */
#define DEFAULT_ERROR_STATUS 404
#define DEFAULT_ERROR_PATH "error.html"

#define DEFAULT_HOST_STATUS 420
#define DEFAULT_HOST_PATH "index.html"

#define DEFAULT_MAX_BODY 1000000

#define DEFAULT_IP_ADRESS "0.0.0.0"
#define DEFAULT_PORT 80

#define ROOT_FOLDER "root/"
/* End of default values */

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

  // Methods
  void prepare(const int backlog = 10);
  void sendResponse(const HttpResponse &response) const;
  void addClient(const int fd);
  void removeClient(const int fd);

  // To check if variables have been set
  bool hasServerName() const;
  bool hasRoutes() const;

  //  Getters
  std::vector<std::string> getServerName() const;
  std::vector<Route> getRoutes() const;
  PageData getHost() const;
  PageData getErrorPage() const;
  int getMaxBody() const;

  int getPort() const;
  std::string getIpAdress() const;
  int getFd() const;
  std::vector<int> &getConnectedClients();
  HttpRequest *getRequestByDescriptor(int fd);
  HttpRequest *getNextRequest() const;
  void removeNextRequest();

  // Setters
  int setPort(int const &value);
  int setIpAddress(std::string const &address);
  int setHost(int const &statusCode, std::string const &filePath);
  int setErrorPage(int const &statusCode, std::string const &filePath);
  void setServerName(std::vector<std::string> const &value);
  int setMaxBody(double const &value);
  void addRoute(Route const &route);
  void addRequest(HttpRequest *request);

private:
  /* Config variables */
  int _port;
  std::string _ipAddress;
  int _maxBodySize;
  std::vector<std::string> _serverName;
  std::vector<HttpRequest *> _unhandledRequests;
  std::vector<int> _connectedClients; // list of connected clients
  std::map<int, HttpRequest *> _requests;
  std::vector<Route> _routes;

  int _fd;               // file descriptor for socket
  int _domain;           // domain of socket (IPv4 or IPv6)
  int _type;             // type of socket (TCP or UDP)
  char _buffer[1000000]; // buffer for reading data from socket
  PageData _defaultErrorPage;
  int _accepted; // file descriptor for accepted connection, -1 if no connection
  PageData _host;
  struct sockaddr_in6 _servaddr; // server address
};
