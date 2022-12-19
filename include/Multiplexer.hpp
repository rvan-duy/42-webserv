#ifndef MULTIPLEXER_HPP
#define MULTIPLEXER_HPP

#include <poll.h>
#include <iterator>
#include <map>
#include <vector>
#include <Server.hpp>
#include <Socket.hpp>
#include <stdexcept>

/****************************************************************************************/
/* The Multiplexer Class                                                                */
/* - The multiplexer class is used to multiplex the file descriptors of the sockets.    */
/****************************************************************************************/

class Multiplexer
{
public:
  Multiplexer(std::vector<Socket> sockets);
  ~Multiplexer();

  void waitForEvents(const int timeout = -1);

private:
  std::vector<Socket> _sockets;
  std::vector<pollfd> _clients; // vector of fds to poll
  bool _endServer;              // flag to end server

  // Methods
  int _pollSockets(const int timeout);
  int _sendData(const int socket, const std::string &data) const;
  int _readData(const int socket, std::string &data) const;
  bool _isSocket(const int fd) const;
  // Socket _getSocketByFd(const int fd) const;
  void _addClient(const int socket);
  void _removeClient(const int socket);
  int _getEvent(const pollfd &fd);
  Socket &_getSocketForClient(const int clientFd);
  void matchRequestToServer(std::vector<Server> const &allServers, HttpRequest *request);

  int evaluateClient(pollfd *client);

  void _addSocketsAsPollFd(short const &events);

  int _processRequest(int const &clientFd, Socket &socket);
};

#endif // MULTIPLEXER_HPP
