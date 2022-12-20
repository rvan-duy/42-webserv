#include <CGI.hpp>
#include <Webserver.hpp>

#include "Multiplexer.hpp"

int initWebserver(std::vector<Socket> *pSockets, std::string const &filePath);

int startWebserver(std::vector<Socket> sockets) {
  Multiplexer multiplexer(sockets);
  try {
    multiplexer.waitForEvents();  // Start the multiplexer loop, does all the
                                  // polling and handling of events etc.
  } catch (std::exception &e) {
    std::cerr << e.what() << std::endl;
    return 1;
  }
  return 0;
}

void testCgi(char **env) {
  CGI testCgi(env);
  std::string dest;
  std::vector<std::string> headers;

  HTTPStatusCode ret =
      testCgi.executeFile(&dest, &headers, "test/cgi/helloworld.py", "yes");
  if (ret == HTTPStatusCode::OK) {
    std::cout << dest << std::endl;
    std::cout << "----" << std::endl;
    for (size_t i = 0; i < headers.size(); i++) {
      std::cout << headers[i] << std::endl;
    }
  } else {
    std::cout << getMessageByStatusCode(ret) << std::endl;
  }
}

int main(int argc, char **argv, char **env) {
  (void)env;
  std::vector<Socket> sockets;
  /*  Input check */
  testCgi(env);
  // if (argc != 2) {
  //   std::cout << "Usage: " << argv[0] << " <config_file>" << std::endl;
  //   return 1;
  // }
  // if (initWebserver(&sockets, argv[1])) return 1;
  // return startWebserver(sockets);
}
