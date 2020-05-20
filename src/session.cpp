#include <common/message.hpp>
#include <iostream>

#include "session.hpp"

/**
 * class to control and store the socket for the connected clients
 */
Session::Session(boost::asio::ip::tcp::socket&& socket,
                 MessageQueue<Message>* pending)
    : _pending(pending) {
  _socket = std::make_unique<boost::asio::ip::tcp::socket>(std::move(socket));
  Read();
};

Session::Session(Session&& other) : _pending(other._pending) {
  _socket = std::move(other._socket);
  // TODO move the _data
};

Session& Session::operator=(Session&& other) {
  _socket = std::move(other._socket);
  // TODO move the _data
  return *this;
};

void Session::Read() {
  _socket->async_read_some(
      _buffer.prepare(514),
      [this](boost::system::error_code ec, std::size_t length) {
        if (!ec) {
          _buffer.commit(length);
          std::istream is(&_buffer);
          Message msg;
          is >> msg;
          _pending->Send(std::move(msg));
          std::cout << "New message!" << std::endl;
        }
        Read();
      });
};
