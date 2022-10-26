#ifndef MULTIPLEXER_HPP
#define MULTIPLEXER_HPP

/****************************************************************************************/
/* The Multiplexer Class                                                                */
/* - The multiplexer class is used to multiplex the file descriptors of the sockets.    */
/****************************************************************************************/

#include <poll.h>

#include "Socket.hpp"

class Multiplexer {
 public:
  Multiplexer();
  ~Multiplexer();

  // Methods
  void add_socket(Socket *socket, const short events);
  void remove_socket(Socket *socket);
  void wait_for_events(const int timeout = -1);

  // Getters
  int get_fd(const int index) const;
  int get_number_of_fds() const;

 private:
  struct pollfd *_fds;            // file descriptors
  int            _number_of_fds;  // number of file descriptors
};

#endif  // MULTIPLEXER_HPP
