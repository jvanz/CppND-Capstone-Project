#include <common/message.hpp>
#include <future>
#include <iostream>

#include "session.hpp"

/**
 * class to control and store the socket for the connected clients
 */
Session::Session(boost::asio::ip::tcp::socket&& socket, Server* server)
    : _server{server} {
  _socket = std::make_unique<boost::asio::ip::tcp::socket>(std::move(socket));
  Read();
};

Session::Session(Session&& other) : _server{other._server} {
  _socket = std::move(other._socket);
  // TODO move the _data
};

Session& Session::operator=(Session&& other) {
  _socket = std::move(other._socket);
  // TODO move the _data
  return *this;
};

void Session::Read() {
  boost::asio::async_read_until(
      *_socket, _buffer, MESSAGE_TERMINATION_CHAR,
      [this](boost::system::error_code ec, std::size_t length) {
        std::cout << "More data! Error: " << ec << ". Length: " << length
                  << " bytes" << std::endl;
        if (!ec) {
          auto bufs = _buffer.data();
          std::istringstream is(
              std::string(boost::asio::buffers_begin(bufs),
                          boost::asio::buffers_begin(bufs) + length));
          _buffer.consume(length);
          Message msg;
          is >> msg;
          auto result = std::async(&Server::ProcessPendingMessage, _server,
                                   std::move(msg), std::ref(*this));
          result.wait();
        }
        Read();
      });
};

void Session::Write(Message& msg) {
  boost::asio::streambuf b;
  std::ostream os(&b);
  os << msg;
  auto bytes_count = _socket->send(b.data());
  std::cout << "Message sent: " << msg.GetID() << ". " << bytes_count
            << " bytes" << std::endl;
}

void Session::Write(Message&& msg) { Write(msg); }

void Session::Listen(Topic& topic) {
  _listeningThreads.emplace_back([this, &topic]() {
    using namespace std::chrono_literals;
    for (;;) {
      auto message = topic.Receive();
      Write(message);
      std::this_thread::sleep_for(1ms);
    }
  });
}
