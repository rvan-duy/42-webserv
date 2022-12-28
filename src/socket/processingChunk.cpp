#include "HttpRequest.hpp"
#include "Socket.hpp"

bool Socket::isChunked(const int &clientFd) {
  Logger::getInstance().debug("is chunked");
  if (_clients[clientFd].first != NULL) {
    Logger::getInstance().debug(_clients[clientFd].first->getHeader("Referer"));
  }
  return _clients[clientFd].first != NULL;
}

/**
 * @brief Adds a chunk of data to an existing HttpRequest object for the client
 * with the specified file descriptor.
 *
 * @param request Pointer to the HttpRequest object containing the chunk of data
 * to be added.
 * @param clientFd The file descriptor of the client to which the HttpRequest
 * object belongs.
 */
void Socket::addChunk(HttpRequest *request, int const &clientFd) {
  Logger::getInstance().log(
      "Adding chunk on client fd: " + std::to_string(clientFd), VERBOSE);
  HttpRequest *original = _clients[clientFd].first;
  _clients[clientFd].first = *original + *request;
}
