#include <boost/asio.hpp>
#include <iostream>

#include "server.hpp"

using boost::asio::ip::tcp;

int main(int argc, char* argv[]) {
  try {
    boost::asio::io_context io_context;
    // start the server which listen for new connections
    Server s(io_context, 8888);
    io_context.run();
  } catch (std::exception& e) {
    std::cerr << "Exception: " << e.what() << "\n";
  }
  return 0;
}
