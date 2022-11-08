#ifndef SOCKET_HPP
#define SOCKET_HPP

#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/ioctl.h>
#include <sys/poll.h>
#include <sys/socket.h>
#include <unistd.h>

#include <cstdlib>
#include <cstring>
#include <fstream>
#include <iostream>
#include <sstream>
#include <vector>

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

#include "HttpRequest.hpp"
#include "HttpResponse.hpp"

#define MAX_PORT 65535

struct Route {
  std::string                          _route;
  std::vector<HttpRequest::HttpMethod> _allowedMethods;
  std::string                          _httpRedirection;
  std::string                          _searchDirectory;
  std::string                          _defaultFile;
  bool                                 _directoryListing;
};

struct PageData {
  int         statusCode;
  std::string filePath;
};

class Server {
 public:
  Server();
  ~Server();

  // Methods
  void prepare(const int backlog = 10);
  void sendResponse(const HttpResponse& response) const;
  bool hasServerName() const;
  bool hasPort() const;
  bool hasMaxBody() const;
  bool hasHost() const;
  bool hasErrorPage() const;
  bool hasRoutes() const;

  // Getters
  int                      getFd() const;
  std::vector<std::string> getServerName() const;
  int                      getPort() const;
  int                      getMaxBody() const;
  PageData                 getHost() const;
  PageData                 getErrorPage() const;

  // Setters
  void setServerName(std::vector<std::string> value);
  int  setPort(int const& value);
  // int  setMaxBody(double const& value); TODO: implement
  int setHost(int const& statusCode, std::string const& filePath);
  int setErrorPage(int const& statusCode, std::string const& filePath);

  // LEGACY METHODS
  void accept_connection();
  void wait_for_connections();

 private:
  int                      _fd;                // file descriptor for socket
  std::vector<std::string> _serverName;        // name of the server
  int                      _domain;            // domain of socket (IPv4 or IPv6)
  int                      _type;              // type of socket (TCP or UDP)
  int                      _port;              // port number of socket (0-65535)
  int                      _maxBodySize;       // ???
  char                     _buffer[1000000];   // buffer for reading data from socket
  int                      _accepted;          // file descriptor for accepted connection, -1 if no connection
  struct sockaddr_in6      _servaddr;          // server address
  std::vector<Route>       _routes;            // list of routes
  PageData                 _defaultErrorPage;  // default error page
  PageData                 _host;              // default host page
};

#endif  // SOCKET_HPP
