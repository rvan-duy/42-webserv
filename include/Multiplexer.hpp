#ifndef MULTIPLEXER_HPP
#define MULTIPLEXER_HPP

#include <poll.h>

#include <iterator>
#include <vector>

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
  void addServer(const int fd, const short events);
  void waitForEvents(const int timeout = -1);

  // Getters
  int getFd(const int index) const;
  int getNumberOfFds() const;

 private:
  std::vector<pollfd> _clients;          // vector of fds to poll
  std::vector<int>    _servers;          // vector of server fds
  char                _buffer[1000000];  // buffer for reading data
  bool                _endServer;        // flag to end server

  // Methods
  int         _pollSockets(const int timeout);
  void        _addClient(const int socket);
  void        _removeClient(const int socket);
  std::string _readData(const int socket);
  int         _getEvent(const pollfd &fd);
};

#endif  // MULTIPLEXER_HPP
