#include <boost/asio.hpp>
#include <iostream>
#include <string>

using boost::asio::ip::tcp;
using namespace std::chrono_literals;

int main(int argc, char* argv[]) {
  try {
    boost::asio::io_context io_context;

    tcp::socket s(io_context);
    tcp::resolver resolver(io_context);
    boost::asio::connect(s, resolver.resolve("localhost", "8888"));

    for (auto x = 0; x < 100; x++) {
      std::string message = "Message " + std::to_string(x);
      std::cout << "Sending: " << message << std::endl;
      boost::asio::write(s, boost::asio::buffer(message));
      std::this_thread::sleep_for(1s);
    }

  } catch (std::exception& e) {
    std::cerr << "Exception: " << e.what() << "\n";
  }

  return 0;
}
