#include <boost/asio.hpp>
#include <iostream>

#include "server.hpp"

using boost::asio::ip::tcp;

int main(int argc, char* argv[]) {
  try {
    // get the data necessary to run
    if (argc != 2) {
      std::cerr << "Invalid arguments. Expected: queue <listen port>"
                << std::endl;
      return -1;
    }
    std::string port(argv[1]);

    boost::asio::io_context io_context;
    // start the server which listen for new connections
    Server s(io_context, std::stoi(port));
    io_context.run();
  } catch (std::exception& e) {
    std::cerr << "Exception: " << e.what() << "\n";
  }
  return 0;
}
