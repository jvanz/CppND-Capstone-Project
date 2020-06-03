#include <boost/asio.hpp>
#include <iostream>
#include <string>

#include <common/message.hpp>

#include "client.hpp"

using boost::asio::ip::tcp;

// Declaration of the default callbacks for the subscribe and read methods
void defaultReadCallback(Client* consumer, Message&& msg);
void defaultSubscribeCallback(Client* consumer, Message&& msg);

// just to keep track of how many messages has been received.
static unsigned int messagesCount = 0;

/**
 * This default callback for the message received from the queue system is print
 * the data into the console.
 */
void defaultReadCallback(Client* consumer, Message&& msg) {
  messagesCount += 1;
  std::cout << "Received: " << msg.GetID() << ": " << msg.GetData()
            << " =--> Total: " << messagesCount << std::endl;
  consumer->Read(defaultReadCallback);
};

/**
 * Default callback to process the response from the queue server about the
 * subscription request.
 *
 * If the server tells the client that's not possible to subscribe, the client
 * try again after one second. If the subscription request succeeds, the client
 * register a callback for all messages sent by the server.
 */
void defaultSubscribeCallback(Client* consumer, Message&& msg) {
  std::cout << "Subscribe callback : " << msg.GetTopic() << " -> "
            << msg.GetData() << "(" << msg.GetID() << ")" << std::endl;
  if (msg.GetType() == MessageType::SUBSCRIBED) {
    // let's print any message received by the server
    consumer->Read(defaultReadCallback);
  } else if (msg.GetType() == MessageType::TOPIC_NOT_FOUND) {
    // no success. Wait a little bit and try again
    std::this_thread::sleep_for(1s);
    consumer->Subscribe(msg.GetTopic(), defaultSubscribeCallback);
  } else {
    std::cout << "Message type not supported" << std::endl;
  }
};

int main(int argc, char* argv[]) {
  // get the data necessary to run
  if (argc != 4) {
    std::cerr << "Invalid arguments. Expected: server port topic" << std::endl;
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
    Client consumer(io_context, server, port);
    // Subscribe to a topic
    consumer.Subscribe(topic, defaultSubscribeCallback);
    io_context.run();
    std::cout << "Bye!" << std::endl;

  } catch (std::exception& e) {
    std::cerr << "Exception: " << e.what() << "\n";
    return -1;
  }

  return 0;
}
