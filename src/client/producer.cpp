#include <boost/asio.hpp>
#include <fstream>
#include <iostream>
#include <string>

#include <common/message.hpp>

#include "client.hpp"

using boost::asio::ip::tcp;
using namespace std::chrono_literals;

// function used to publish messages
static void publishMessage(Client* producer, Message&& confirmationMessage,
                           std::string& messagesfilepath) {
  std::ifstream messagesfile(messagesfilepath);
  if (messagesfile.is_open()) {
    for (std::string line; std::getline(messagesfile, line);) {
      producer->PublishMessage(confirmationMessage.GetTopic(), line);
    }
  }
}

int main(int argc, char* argv[]) {
  // get the data necessary to run
  if (argc != 5) {
    std::cerr << "Invalid arguments. Expected: producer <server address> "
                 "<server port> <topic to publish messages>"
              << std::endl;
    return -1;
  }
  std::string server(argv[1]);
  std::string port(argv[2]);
  std::string topic(argv[3]);
  std::string messagesfilepath(argv[4]);

  try {
    boost::asio::io_context io_context;
    // prevent the context to stop if there are no events in the loop
    boost::asio::executor_work_guard<boost::asio::io_context::executor_type>
        work = boost::asio::make_work_guard(io_context);

    // Connect to the queue server
    Client producer(io_context, server, port);

    // Create the topic
    producer.CreateTopic(
        topic, [&messagesfilepath](Client* client, Message&& message) {
          if (message.GetType() == MessageType::TOPIC_CREATED) {
            // topic created. Start a task to publish the messages
            std::cout << "Topic created" << std::endl;
            auto future = std::async(publishMessage, client, std::move(message),
                                     std::ref(messagesfilepath));
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
