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
  Logger &logger  = Logger::getInstance();
  fds    *new_fds = new fds[_number_of_fds + 1];

  logger.log("Adding socket to multiplexer");

  for (int i = 0; i < _number_of_fds; i++) {
    new_fds[i].fd      = _fds[i].fd;
    new_fds[i].events  = _fds[i].events;
    new_fds[i].revents = _fds[i].revents;
    new_fds[i].socket  = _fds[i].socket;
  }

  new_fds[_number_of_fds].fd      = socket->get_fd();
  new_fds[_number_of_fds].events  = events;
  new_fds[_number_of_fds].revents = 0;  // nothing for now, maybe later
  new_fds[_number_of_fds].socket  = socket;

  if (_fds != nullptr) {
    delete[] _fds;
  }

  _fds = new_fds;
  _number_of_fds++;

  logger.log("Socket added to multiplexer, total number of sockets: " + std::to_string(_number_of_fds));
}

/*
 * Remove a socket from the multiplexer
 * @param socket Socket to be removed
 */
void Multiplexer::remove_socket(Socket *socket) {
  Logger &logger  = Logger::getInstance();
  fds    *new_fds = new fds[_number_of_fds - 1];

  logger.log("Removing socket from multiplexer");

  for (int i = 0, j = 0; i < _number_of_fds; i++) {
    if (_fds[i].socket != socket) {
      new_fds[j].fd      = _fds[i].fd;
      new_fds[j].events  = _fds[i].events;
      new_fds[j].revents = _fds[i].revents;
      new_fds[j].socket  = _fds[i].socket;
      j++;
    }
  }

  if (_fds != nullptr) {
    delete[] _fds;
  }

  _fds = new_fds;
  _number_of_fds--;

  logger.log("Socket removed from multiplexer, total number of sockets: " + std::to_string(_number_of_fds));
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

  logger.log("poll(): Waiting for events with timeout: " + std::to_string(timeout) + " ms");
  {
    logger.log("poll(): Number of file descriptors: " + std::to_string(_number_of_fds));
    // poll() returns the number of file descriptors with events
    int poll_result = poll((struct pollfd *)_fds, _number_of_fds, timeout);

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
      _fds[i].socket->accept_connection();
    }
  }
}
