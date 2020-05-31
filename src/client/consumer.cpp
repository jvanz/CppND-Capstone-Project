#include <boost/asio.hpp>
#include <iostream>
#include <string>

#include <common/message.hpp>

using boost::asio::ip::tcp;
using namespace std::chrono_literals;

class Consumer {
 public:
  Consumer(tcp::socket&& socket)
      : _socket(std::move(socket)), _message_received{0} {
    Read();
    Subscribe();
  }

 private:
  void Subscribe() {
    std::ostream os(&_write_buffer);
    Message msg(MessageType::SUBSCRIBE, "test");
    os << msg;
    boost::asio::write(_socket, _write_buffer);
  };

  void Read() {
    boost::asio::async_read_until(
        _socket, _read_buffer, MESSAGE_TERMINATION_CHAR,
        [this](boost::system::error_code ec, std::size_t length) {
          if (!ec) {
            auto bufs = _read_buffer.data();
            std::istringstream is(
                std::string(boost::asio::buffers_begin(bufs),
                            boost::asio::buffers_begin(bufs) + length));
            Message msg;
            is >> msg;
            _message_received += 1;
            _read_buffer.consume(length);
            std::cout << "Received: " << msg.GetID() << ": " << msg.GetData()
                      << " =--> Total: " << _message_received << std::endl;
          }
          Read();
        });
  };

  boost::asio::streambuf _read_buffer;
  boost::asio::streambuf _write_buffer;
  tcp::socket _socket;
  unsigned int _message_received;
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
