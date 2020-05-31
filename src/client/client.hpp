#include <boost/asio.hpp>
#include <iostream>
#include <string>

#include <common/message.hpp>

using boost::asio::ip::tcp;
using namespace std::chrono_literals;

/**
 * Class to stablish and send message to queue system
 */
class Client {
 public:
  Client(tcp::socket&& socket) : _socket(std::move(socket)){};

  void Subscribe(std::string topic);
  void Publish(std::string topic, std::string data);
  void Receive(std::string topic);

 private:
  void Write();
  void Read();

  boost::asio::streambuf _read_buffer;
  boost::asio::streambuf _write_buffer;
  tcp::socket _socket;
};
