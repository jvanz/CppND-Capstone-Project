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
    auto length = boost::asio::write(s, b);
    std::cout << "Sent: " << msg.GetID() << ". Length: " << length << std::endl;

    for (auto x = 0; x < 100; x++) {
      auto time = 100ms;
      std::this_thread::sleep_for(time);
      boost::asio::streambuf b2;
      std::ostream os2(&b2);
      Message msg2(MessageType::SEND, "test", "message " + std::to_string(x));
      os2 << msg2;
      length = boost::asio::write(s, b2);
      std::cout << "Sent: " << msg2.GetID() << ". Length: " << length + 1
                << ". Count: " << x << std::endl;
    }

    boost::asio::streambuf read_buffer;
    s.async_read_some(
        read_buffer.prepare(512),
        [&read_buffer](boost::system::error_code ec, std::size_t length) {});
    io_context.run();

  } catch (std::exception& e) {
    std::cerr << "Exception: " << e.what() << "\n";
  }

  return 0;
}
