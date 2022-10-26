#include "Multiplexer.hpp"

#include "Logger.hpp"

Multiplexer::Multiplexer() : _fds(nullptr), _number_of_fds(0) {}

Multiplexer::~Multiplexer() {
  if (_fds != nullptr) {
    delete[] _fds;
  }
}

/*
 * Add a socket to the multiplexer
 * @param socket Socket to be added
 */
void Multiplexer::add_socket(Socket *socket, const short events) {
  struct pollfd *new_fds = new struct pollfd[_number_of_fds + 1];

  for (int i = 0; i < _number_of_fds; i++) {
    new_fds[i] = _fds[i];
  }

  new_fds[_number_of_fds].fd     = socket->get_fd();
  new_fds[_number_of_fds].events = events;

  if (_fds != nullptr) {
    delete[] _fds;
  }

  _fds = new_fds;
  _number_of_fds++;
}

/*
 * Remove a socket from the multiplexer
 * @param socket Socket to be removed
 */
void Multiplexer::remove_socket(Socket *socket) {
  struct pollfd *new_fds = new struct pollfd[_number_of_fds - 1];

  int            j       = 0;
  for (int i = 0; i < _number_of_fds; i++) {
    if (_fds[i].fd != socket->get_fd()) {
      new_fds[j] = _fds[i];
      j++;
    }
  }

  if (_fds != nullptr) {
    delete[] _fds;
  }

  _fds = new_fds;
  _number_of_fds--;
}

/*
 * Wait for events on the sockets
 * @param timeout Timeout in milliseconds, -1 for infinite
 */
void Multiplexer::wait_for_events(const int timeout) {
  Logger &logger = Logger::getInstance();

  /**************************************************/
  /* poll() waits for an event on a file descriptor */
  /**************************************************/

  {
    int poll_result = poll(_fds, _number_of_fds, timeout);  // poll() returns the number of file descriptors with events

    if (poll_result == -1) {
      logger.error("poll() failed: " + std::string(strerror(errno)));
      throw std::runtime_error("poll() failed: " + std::string(strerror(errno)));
    } else if (poll_result == 0) {
      logger.error("poll() timed out: " + std::to_string(timeout) + " ms");
      throw std::runtime_error("poll() timed out: " + std::to_string(timeout) + " ms");
    } else {
      logger.log("poll() returned: " + std::to_string(poll_result));
    }
  }

  /**************************************************/
  /* Check for events on the sockets                */
  /**************************************************/

  for (int i = 0; i < _number_of_fds; i++) {
    if (_fds[i].revents & POLLIN) {
      logger.log("POLLIN event on fd " + std::to_string(_fds[i].fd));
    }
  }
}
