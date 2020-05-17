#include <iostream>
#include <memory>
#include <utility>

#include "server.hpp"

Server::Server(boost::asio::io_context& io_context, short port)
    : _acceptor(io_context, tcp::endpoint(tcp::v4(), port)) {
  _acceptor.async_accept(
      [this](boost::system::error_code ec, tcp::socket socket) {
	if (!ec) {
	  std::cout << "Accepted new connection!" << std::endl;
	  _sessions.push_back(std::make_shared<Session>(std::move(socket)));
	}
      });
};
