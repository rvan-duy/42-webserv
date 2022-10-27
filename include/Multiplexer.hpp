#ifndef MULTIPLEXER_HPP
#define MULTIPLEXER_HPP

#include <poll.h>

#include "Socket.hpp"

/***************************************************************************************/
/* file_descriptor struct                                                              */
/* - The file_descriptor structure is used to store file descriptor information for    */
/*   the poll() function.                                                              */
/* - it can be cast to a pollfd structure.                                             */
/***************************************************************************************/

typedef struct file_descriptor {
  int     fd;       // file descriptor
  short   events;   // requested events
  short   revents;  // returned events
  Socket *socket;   // socket
} fds;

/****************************************************************************************/
/* The Multiplexer Class                                                                */
/* - The multiplexer class is used to multiplex the file descriptors of the sockets.    */
/****************************************************************************************/

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
  fds *_fds;            // file descriptors
  int  _number_of_fds;  // number of file descriptors
};

#endif  // MULTIPLEXER_HPP
