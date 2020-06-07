#include <boost/asio.hpp>
#include <iostream>
#include <string>

#include <common/message.hpp>

#include "client.hpp"

using boost::asio::ip::tcp;
using namespace std::chrono_literals;

// total number of messages send by the program to the queue system
constexpr unsigned int TOTAL_MESSAGES_COUNT = 1000;

// function used to publish messages
static void publishMessage(Client* producer, Message&& confirmationMessage) {
  for (auto x = 0; x < TOTAL_MESSAGES_COUNT; x++) {
    auto time = 10ms;
    std::this_thread::sleep_for(time);
    producer->PublishMessage(confirmationMessage.GetTopic(),
                             "message " + std::to_string(x));
  }
}

int main(int argc, char* argv[]) {
  // get the data necessary to run
  if (argc != 4) {
    std::cerr << "Invalid arguments. Expected: producer <server address> "
                 "<server port> <topic to publish messages>"
              << std::endl;
    return -1;
  }
  std::string server(argv[1]);
  std::string port(argv[2]);
  std::string topic(argv[3]);

  try {
    boost::asio::io_context io_context;
    // prevent the context to stop if there are no events in the loop
    boost::asio::executor_work_guard<boost::asio::io_context::executor_type>
        work = boost::asio::make_work_guard(io_context);

    // Connect to the queue server
    Client producer(io_context, server, port);

    // Create the topic
    producer.CreateTopic(topic, [](Client* client, Message&& message) {
      if (message.GetType() == MessageType::TOPIC_CREATED) {
        // topic created. Start a task to publish the messages
        std::cout << "Topic created" << std::endl;
        auto future = std::async(publishMessage, client, std::move(message));
        future.wait();
      } else if (message.GetType() == MessageType::TOPIC_CREATED) {
        std::cout << "Cannot create the topic" << std::endl;
      } else {
        std::cout << "Message type not supported" << std::endl;
      }
    });

    io_context.run();
    std::cout << "Bye!" << std::endl;

  } catch (std::exception& e) {
    std::cerr << "Exception: " << e.what() << "\n";
    return -1;
  }

  return 0;
}
