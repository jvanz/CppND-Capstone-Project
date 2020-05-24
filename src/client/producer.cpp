#include <boost/asio.hpp>
#include <iostream>
#include <string>

#include <common/message.hpp>

using boost::asio::ip::tcp;
using namespace std::chrono_literals;

int main(int argc, char* argv[]) {
  try {
    boost::asio::io_context io_context;

    tcp::socket s(io_context);
    tcp::resolver resolver(io_context);
    boost::asio::connect(s, resolver.resolve("localhost", "8888"));

    boost::asio::streambuf b;
    std::ostream os(&b);
    Message msg(MessageType::CREATE, "test");
    os << msg;
    boost::asio::write(s, b);

    for (auto x = 0; x < 10; x++) {
      // std::this_thread::sleep_for(1s);
      boost::asio::streambuf b2;
      std::ostream os2(&b2);
      Message msg2(MessageType::SEND, "test", "message " + std::to_string(x));
      os2 << msg2;
      std::cout << "Send: " << msg.GetID() << std::endl;
      boost::asio::write(s, b2);
    }
    io_context.run();

  } catch (std::exception& e) {
    std::cerr << "Exception: " << e.what() << "\n";
  }

  return 0;
}
