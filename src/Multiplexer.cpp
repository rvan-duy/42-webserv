#include "Multiplexer.hpp"

#include "Logger.hpp"

Multiplexer::Multiplexer() : _number_of_fds(0), _end_server(false) {
  memset(&_fds, 0, sizeof(_fds));
  memset(&_buffer, 0, sizeof(_buffer));
}

Multiplexer::~Multiplexer() {}

/*
 * Add a server to the multiplexer
 * @param fd The file descriptor of the server
 * @param events The events to listen for
 */
void Multiplexer::add_server(const int fd, const short events) {
  Logger &logger = Logger::getInstance();

  logger.log("Adding server to multiplexer: " + std::to_string(fd));

  _servers.push_back(fd);
  _fds.push_back({fd, events, (short)0});

  logger.log("Server added to multiplexer, total number of stored sockets: " + std::to_string(_fds.size()));
}

/*
 * Wait for events on the sockets
 * @param timeout Timeout in milliseconds, -1 for infinite
 */
void Multiplexer::wait_for_events(const int timeout) {
  Logger &logger = Logger::getInstance();

  logger.log("Starting poll() loop with timeout of " + std::to_string(timeout) + " milliseconds");
  do {
    if (_poll_sockets(timeout) == -1) break;

    /**************************************************/
    /* Poll has successfully returned, this means a   */
    /* socket has an event to handle.                 */
    /**************************************************/

    {
      logger.log("Poll returned successfully, checking for events on sockets");
      const int number_of_sockets_to_check = _fds.size();
      for (int i = 0; i < number_of_sockets_to_check; i++) {  // Loop through all sockets inside the multiplexer
        int event_type = _get_event(_fds[i]);                 // Logging is done inside the _get_event() method

        /**************************************************/
        /* Main switch to handle the different events     */
        /**************************************************/
        switch (event_type) {
          /**************************************************/
          /* No event                                       */
          /**************************************************/
          case 0: {
            break;
          }

          /**************************************************/
          /* Error event                                    */
          /**************************************************/
          case POLLERR: {
            logger.error("CODE NOT IMPLEMENTED YET");
            break;
          }

          /**************************************************/
          /* Hangup event                                   */
          /**************************************************/
          case POLLHUP: {
            logger.error("CODE NOT IMPLEMENTED YET");
            break;
          }

          /**************************************************/
          /* Read event                                     */
          /**************************************************/
          case POLLIN: {
            if (std::find(_servers.begin(), _servers.end(), _fds[i].fd) != _servers.end()) {
              // If the fd is a server, accept the connection one of the clients
              add_client(_fds[i].fd);
            } else {
              // if its already a client, read the data
              std::string data = read_data(_fds[i].fd);
              if (data.empty()) {
                // If the data is empty, the client has disconnected
                remove_client(_fds[i].fd);
                // close ?????
              } else {
                // If the data is not empty, handle the data
                logger.error("CODE NOT IMPLEMENTED YET");
              }
            }
            // 3.1. If the data is read, build a request object and later on a response object
            // 3.2. If the data is not read, log an error and close the socket
            break;
          }

          /**************************************************/
          /* Write event                                    */
          /**************************************************/
          case POLLOUT: {
            logger.error("CODE NOT IMPLEMENTED YET");
            break;
          }

          /**************************************************/
          /* Default event                                  */
          /**************************************************/
          default: {
            logger.error("CODE NOT IMPLEMENTED YET");
            break;
          }
        }
      }
    }

  } while (_end_server == false);

  do {
    /**************************************************/
    /* When the code gets here events have occurred,  */
    /* we need to determine which events have         */
    /* occurred.                                      */
    /**************************************************/

    logger.log("Events have occurred on sockets");
    int current_size = _number_of_fds;
    for (int i = 0; i < current_size; i++) {
      if (_fds[i].revents == 0) {
        continue;
      }

      if (_fds[i].revents & POLLIN) {
        logger.log("POLLIN event occurred on socket " + std::to_string(_fds[i].fd));

        // for now this is hardcoded to accept only one connection
        if (_fds[i].fd == _listening_fd[0]) {
          do {
            new_sd = accept(_listening_fd[0], nullptr, nullptr);
            if (new_sd < 0) {
              if (errno != EWOULDBLOCK) {
                logger.error("accept() failed");
                perror("accept()");
                end_server = true;
              }
              break;
            }
            logger.log("New connection accepted on socket " + std::to_string(new_sd));
            _fds[_number_of_fds].fd     = new_sd;
            _fds[_number_of_fds].events = POLLIN;
            _number_of_fds++;
          } while (new_sd != -1);
        } else {
          bool close_connection_needed = false;
          logger.log(std::to_string(_fds[i].fd) + " is ready to read");
          do {
            int ret = recv(_fds[i].fd, _buffer, sizeof(_buffer), 0);
            if (ret < 0) {
              if (errno != EWOULDBLOCK) {
                logger.error("read() failed");
                perror("read()");
                close_connection_needed = true;
              }
              break;
            }
            if (ret == 0) {
              logger.log("Connection closed on socket " + std::to_string(_fds[i].fd));
              close_connection_needed = true;
              break;
            }
            logger.log("Received " + std::to_string(ret) + " bytes on socket " + std::to_string(_fds[i].fd));
            ret = send(_fds[i].fd, _buffer, ret, 0);
            if (ret < 0) {
              logger.error("send() failed");
              perror("send()");
              close_connection_needed = true;
              break;
            }
            close(_fds[i].fd);
          } while (true);

          if (close_connection_needed == true) {
            close(_fds[i].fd);
            _fds[i].fd = -1;
          }
        }
      } else {
        logger.error("Unknown event occurred on socket " + std::to_string(_fds[i].fd));
        end_server = true;
        break;
      }
    }
  } while (end_server == false);
}

/*
 * Add a client socket to the multiplexer, so we can listen for events on it
 * @param socket The socket to add to the multiplexer
 */
void Multiplexer::add_client(const int socket) {
  Logger &logger = Logger::getInstance();

  logger.log("Adding server socket " + std::to_string(socket) + " to multiplexer");
  int new_socket = accept(socket, nullptr, nullptr);
  if (new_socket == -1) {
    logger.error("Failed to accept new connection: " + std::string(strerror(errno)));
    _end_server = true;
    return;
  }
  logger.log("New connection accepted: " + std::to_string(new_socket));
  _fds.push_back({new_socket, POLLIN, (short)0});
  logger.log("New connection added to multiplexer, total number of stored sockets: " + std::to_string(_fds.size()));
}

/*
 * Remove a client socket from the multiplexer
 * @param socket The socket to remove from the multiplexer
 */
void Multiplexer::remove_client(const int socket) {
  Logger &logger = Logger::getInstance();

  logger.log("Removing client socket " + std::to_string(socket) + " from multiplexer");
  for (std::iterator it = _fds.begin(); it != _fds.end(); ++it) {
    if (it->fd == socket) {
      _fds.erase(it);
      break;
    }
  }
  logger.log("Client socket removed from multiplexer, total number of stored sockets: " + std::to_string(_fds.size()));
}

/*
 * Reads data from a socket and returns it as a string
 * @param socket The socket to read from
 */
std::string Multiplexer::read_data(const int socket) {
  Logger &logger = Logger::getInstance();

  logger.log("Reading data from socket " + std::to_string(socket));
  int bytes_read = read(socket, _buffer, sizeof(_buffer));
  if (bytes_read == -1) {
    logger.error("Failed to read data from socket " + std::to_string(socket));
    _end_server = true;
    return "";
  } else if (bytes_read == 0) {
    logger.log("Connection closed on socket " + std::to_string(socket));
    logger.error("CODE NOT IMPLEMENTED YET");
    return "";
  }
  logger.log("Read " + std::to_string(bytes_read) + " bytes from socket " + std::to_string(socket));
  return std::string(_buffer, bytes_read);
}

/*
 * Check for an return event on a socket
 * @param fd The pollfd to check
 */
int Multiplexer::_get_event(const pollfd &fd) {
  Logger &logger = logger::getInstance();

  logger.log("Checking for event on socket " + std::to_string(fd.fd));
  if (fd.revents == 0) {
    logger.log("No event on socket " + std::to_string(fd.fd));
    return 0;
  } else if (fd.revents & POLLIN) {
    logger.log("POLLIN event on socket " + std::to_string(fd.fd));
    return POLLIN;
  } else if (fd.revents & POLLOUT) {
    logger.log("POLLOUT event on socket " + std::to_string(fd.fd));
    return POLLOUT;
  } else {
    logger.log("Unknown event on socket " + std::to_string(fd.fd));
    return POLLERR;
  }
}

/*
 * Poll the sockets
 * @param timeout Timeout in milliseconds, -1 for infinite
 * @return 0 on success, -1 on error
 */
int Multiplexer::_poll_sockets(const int timeout) {
  Logger &logger = Logger::getInstance();
  pollfd *fds    = &_fds[0];

  logger.log("Polling for events on " + std::to_string(_fds.size()) + " sockets...");
  int poll_result = poll(fds, _fds.size(), timeout);
  switch (poll_result) {
    case -1:
      logger.error("poll() returned -1, error occurred: " + std::string(strerror(errno)));
      return -1;
    case 0:
      logger.log("poll() returned 0, timeout occurred");
      return -1;
    default:
      logger.log("Events have occurred on sockets");
      return 0;
  }
}
