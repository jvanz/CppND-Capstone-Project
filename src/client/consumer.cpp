#include <boost/asio.hpp>
#include <iostream>
#include <string>

#include <common/message.hpp>

using boost::asio::ip::tcp;
using namespace std::chrono_literals;
void read(boost::system::error_code ec, std::size_t length);

class Consumer {
 public:
  Consumer(tcp::socket&& socket) : _socket(std::move(socket)) {
    Read();
    Subscribe();
  }

 private:
  void Subscribe() {
    std::ostream os(&_write_buffer);
    Message msg(MessageType::SUBSCRIBE, "test");
    os << msg;
    std::cout << "Message sent: " << msg << std::endl;
    boost::asio::write(_socket, _write_buffer);
  };

  void Read() {
    _socket.async_read_some(
        _read_buffer.prepare(514),
        [this](boost::system::error_code ec, std::size_t length) {
          if (!ec) {
            std::istream is(&_read_buffer);
            _read_buffer.commit(length);
            Message msg;
            is >> msg;
            std::cout << "Received: " << msg << std::endl;
          }
          Read();
        });
  };

  boost::asio::streambuf _read_buffer;
  boost::asio::streambuf _write_buffer;
  tcp::socket _socket;
};

int main(int argc, char* argv[]) {
  try {
    boost::asio::io_context io_context;
    tcp::resolver resolver(io_context);
    tcp::socket socket(io_context);
    boost::asio::connect(socket, resolver.resolve("localhost", "8888"));

    Consumer consumer(std::move(socket));
    io_context.run();

  } catch (std::exception& e) {
    std::cerr << "Exception: " << e.what() << "\n";
  }

  return 0;
}