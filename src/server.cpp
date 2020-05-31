#include <chrono>
#include <iostream>
#include <memory>
#include <utility>

#include "server.hpp"

Server::Server(boost::asio::io_context &io_context, short port)
    : _acceptor(io_context, tcp::endpoint(tcp::v4(), port)) {
  Accept();
};

void Server::Accept() {
  _acceptor.async_accept([this](boost::system::error_code ec,
                                tcp::socket socket) {
    if (!ec) {
      std::cout << "Accepted new connection!" << std::endl;
      _sessions.push_back(std::make_unique<Session>(std::move(socket), this));
    }
    Accept();
  });
};

void Server::AddTopic(Topic &&topic) {
  std::lock_guard<std::mutex> lck(_topicsMtx);
  _topics[topic.GetName()] = std::make_shared<Topic>(topic);
  std::cout << "Topic created: " << topic.GetName()
            << ". Topic count: " << _topics.size() << std::endl;
}

Topic *Server::FindTopic(std::string topicName) {
  std::lock_guard<std::mutex> lck(_topicsMtx);
  if (_topics.find(topicName) != _topics.end()) return _topics[topicName].get();
  return nullptr;
}

void Server::ProcessPendingMessage(Message &&message, Session &session) {
  using namespace std::chrono_literals;
  auto time = 50ms;
  std::this_thread::sleep_for(time);

  std::cout << "Processing: "
            << "(" << message.GetID() << ")"
            << " Type=" << message.GetType()
            << ", topic = '" + message.GetTopic() << "'"
            << ", data = '" << message.GetData() << "'" << std::endl;
  if (message.GetType() == MessageType::CREATE) {
    std::cout << "Creating a new topic" << std::endl;
    AddTopic(Topic(message.GetData()));
  } else if (message.GetType() == MessageType::SEND) {
    auto topic = FindTopic(message.GetTopic());
    if (topic) {
      topic->Send(std::move(message));
    } else {
      std::cout << "Topic " << message.GetTopic() << " not found." << std::endl;
    }
  } else if (message.GetType() == MessageType::SUBSCRIBE) {
    std::cout << "Subscription request: " << message.GetData() << std::endl;
    auto topic = FindTopic(message.GetData());
    if (topic) {
      session.Listen(*topic);
    } else {
      std::cout << "Topic " << message.GetTopic() << " not found." << std::endl;
    }
  }
}
