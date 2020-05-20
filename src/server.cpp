#include <chrono>
#include <iostream>
#include <memory>
#include <utility>

#include "server.hpp"

Server::Server(boost::asio::io_context &io_context, short port)
    : _acceptor(io_context, tcp::endpoint(tcp::v4(), port)) {
  _pendingMessages = std::make_unique<MessageQueue<Message>>();
  Accept();
  StartThreads();
};

Server::~Server() {
  for (auto &thread : _threads) thread.join();
}

void Server::Accept() {
  _acceptor.async_accept(
      [this](boost::system::error_code ec, tcp::socket socket) {
        if (!ec) {
          std::cout << "Accepted new connection!" << std::endl;
          _sessions.push_back(std::make_shared<Session>(
              std::move(socket), _pendingMessages.get()));
        }
        Accept();
      });
};

void Server::StartThreads() {
  // start a thread to process the pending message
  _threads.emplace_back([this]() {
    // TODO how to stop the thread?
    using namespace std::chrono_literals;
    for (;;) {
      std::this_thread::sleep_for(5s);
      auto message = _pendingMessages->Receive();
      std::cout << "Processing: Type=" << message.GetType()
                << ", data = " << message.GetData() << std::endl;
    }
  });
};
