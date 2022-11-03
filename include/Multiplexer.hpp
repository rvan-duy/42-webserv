#ifndef MULTIPLEXER_HPP
#define MULTIPLEXER_HPP

#include <poll.h>

#include <vector>

#include "Socket.hpp"

/****************************************************************************************/
/* The Multiplexer Class                                                                */
/* - The multiplexer class is used to multiplex the file descriptors of the sockets.    */
/****************************************************************************************/

class Multiplexer {
 public:
  Multiplexer();
  ~Multiplexer();

  // Methods
  void add_server(const int fd, const short events);
  void wait_for_events(const int timeout = -1);

  // Getters
  int get_fd(const int index) const;
  int get_number_of_fds() const;

 private:
  // maybe rename _fds to _clients
  std::vector<pollfd> _fds;              // vector of fds to poll
  std::vector<int>    _servers;          // vector of server fds
  char                _buffer[1000000];  // buffer for reading data
  bool                _end_server;       // flag to end server

  // Methods
  int  _poll_sockets(const int timeout);
  void _get_event(const pollfd &fd);
};

#endif  // MULTIPLEXER_HPP
