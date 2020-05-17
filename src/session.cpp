#include <iostream>

#include "session.hpp"

/**
 * class to control and store the socket for the connected clients
 */
Session::Session(boost::asio::ip::tcp::socket&& socket)
    : _socket(std::move(socket)) {
  Read();
};

void Session::Read() {
  _socket.async_read_some(
      boost::asio::buffer(_data, _max_length),
      [this](boost::system::error_code ec, std::size_t length) {
	if (!ec) {
	  std::cout << std::string_view(_data, length) << std::endl;
	}
	Read();
      });
}
