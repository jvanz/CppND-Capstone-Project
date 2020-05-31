
#include "client.hpp"

void Client::Subscribe(std::string topic, ) {
  Message msg(MessageType::SUBSCRIBE, topic);
  Write(std::move(msg));
}

void Client::Publish(std::string topic, std::string data) {}

Message&& Client::(std::string topic) {}

void Client::Write(Message&& msg) {
  std::ostream os(&_write_buffer);
  os << msg;
  std::cout << "Message sent: " << msg << std::endl;
  boost::asio::write(_socket, _write_buffer);
}

void Client::Read() {
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
}
