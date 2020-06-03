#include "client.hpp"

Client::Client(boost::asio::io_context& io_context, std::string server,
               std::string port)
    : _messagesReadCount(0), _messagesWriteCount(0), _socket(io_context) {
  tcp::resolver resolver(io_context);
  boost::asio::connect(_socket, resolver.resolve(server, port));
};

void Client::Subscribe(std::string topic, ReadCallback callback) {
  SubscriptionRequestMessage msg(topic);
  Read(callback);
  Write(std::move(msg));
};

void Client::Read(ReadCallback callback) {
  std::cout << "Read()" << std::endl;
  boost::asio::async_read_until(
      _socket, _read_buffer, MESSAGE_TERMINATION_CHAR,
      [this, callback](boost::system::error_code ec, std::size_t length) {
        std::cout << "More data! Error: " << ec << ". Length: " << length
                  << " bytes" << std::endl;
        if (!ec) {
          auto bufs = _read_buffer.data();
          std::istringstream is(
              std::string(boost::asio::buffers_begin(bufs),
                          boost::asio::buffers_begin(bufs) + length));
          Message msg;
          is >> msg;
          _read_buffer.consume(length);
          _messagesReadCount += 1;
          callback(this, std::move(msg));
        }
      });
};

void Client::CreateTopic(std::string topic, ReadCallback callback) {
  CreateTopicMessage msg(topic);
  Read(callback);
  Write(std::move(msg));
}

void Client::PublishMessage(std::string topic, std::string data) {
  Message message(MessageType::SEND, topic, data);
  Write(std::move(message));
}

void Client::Write(Message&& message) {
  boost::asio::streambuf buffer;
  std::ostream os2(&buffer);
  os2 << message;
  auto length = boost::asio::write(_socket, buffer);
  _messagesWriteCount += 1;
  std::cout << "Sent: " << message.GetID() << ". Length: " << length + 1
            << ". Count: " << _messagesWriteCount << std::endl;
}
