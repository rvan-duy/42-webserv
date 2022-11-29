#ifndef MULTIPLEXER_HPP
#define MULTIPLEXER_HPP

#include <poll.h>

#include <iterator>
#include <vector>
#include <map>

#include "Server.hpp"

/****************************************************************************************/
/* The Multiplexer Class                                                                */
/* - The multiplexer class is used to multiplex the file descriptors of the sockets.    */
/****************************************************************************************/

class Multiplexer {
 public:
  Multiplexer();
  ~Multiplexer();

  // Methods
  void addServer(const Server &server, const short events);
  void waitForEvents(const int timeout = -1);

  // Getters
  int getFd(const int index) const;
  int getNumberOfFds() const;

 private:
  std::vector<pollfd>        _clients;    // vector of fds to poll
  std::vector<Server>        _servers;    // vector of servers
  bool                       _endServer;  // flag to end server

  // Methods
  Server&   _getServerForClient(int fd);
  int    _pollSockets(const int timeout);
  int    _sendData(const int socket, const std::string &data) const;
  int    _readData(const int socket, std::string &data) const;
  bool   _isServer(const int fd) const;
  Server _getServer(const int fd) const;
  void   _addClient(const int socket);
  void   _removeClient(const int socket);
  int    _getEvent(const pollfd &fd);
};

#endif  // MULTIPLEXER_HPP
