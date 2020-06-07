#include <common/message.hpp>
#include <exception>
#include <future>
#include <iostream>
#include <stdexcept>

#include "session.hpp"

/**
 * class to control and store the socket for the connected clients
 */
Session::Session(boost::asio::ip::tcp::socket&& socket, Server* server)
    : _server{server} {
  _socket = std::make_unique<boost::asio::ip::tcp::socket>(std::move(socket));
  Read();
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
          auto result =
              std::async(std::launch::async, &Server::ProcessPendingMessage,
                         _server, std::move(msg), std::ref(*this));
          result.wait();
        } else {
          std::cerr << "ERROR: " << std::endl;
          return;
        }
        Read();
      });
};

void Session::Write(Message& msg) {
  boost::asio::streambuf b;
  std::ostream os(&b);
  os << msg;
  try {
    auto bytes_count = _socket->send(b.data());
    std::cout << "Message sent: " << msg.GetID() << ". " << bytes_count
              << " bytes -> Type: " << msg.GetType()
              << ", topic: " << msg.GetTopic() << ", data: " << msg.GetData()
              << std::endl;
  } catch (...) {
    // It's not possible to send the message.Shutdown the socket
    std::cerr << "Cound not send: " << msg.GetData() << std::endl;
    _socket->shutdown(boost::asio::ip::tcp::socket::shutdown_both);
    _socket->close();
    std::rethrow_exception(std::current_exception());
  }
}

void Session::Write(Message&& msg) { Write(msg); }

void Session::Listen(Topic& topic) {
  _listeningThreads.emplace_back([this, &topic]() {
    using namespace std::chrono_literals;
    while (_socket->is_open()) {
      auto message = topic.Receive();
      try {
        Write(message);
      } catch (...) {
        // It's not possible to send the message. Insert it again in the queue
        // to be processed
        topic.Send(std::move(message));
        return;
      }
      std::this_thread::sleep_for(1ms);
    }
  });
}
