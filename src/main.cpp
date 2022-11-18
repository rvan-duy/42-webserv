#include "Multiplexer.hpp"
#include <Webserver.hpp>

void startWebserver(std::vector<Server> servers)
{
  Multiplexer multiplexer;

  try
  {
    // Iterate through all servers and add them to the multiplexer
    for (std::vector<Server>::iterator it = servers.begin(); it != servers.end(); ++it)
    {
      it->prepare(); // socket() + bind() + listen()
      multiplexer.addServer(*it, POLLIN);
    }
    multiplexer.waitForEvents(); // Start the multiplexer loop, does all the polling and handling of events etc.
  }
  catch (std::exception &e)
  {
    std::cerr << e.what() << std::endl;
  }
}

int main(int argc, char **argv)
{
  std::vector<Server> servers;
  /*  Input check */
  if (argc != 2)
  {
    std::cout << "Usage: " << argv[0] << " <config_file>" << std::endl;
    return 1;
  }
  if (initWebserver(&servers, argv[1]))
  {
    return 1;
  }
  startWebserver(servers);
  return 0;
}
