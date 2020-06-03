#include <boost/asio.hpp>
#include <iostream>
#include <string>

#include <common/message.hpp>

using boost::asio::ip::tcp;
using namespace std::chrono_literals;

class Client;
// Callback function type used in the Client class methods
using ReadCallback = std::function<void(Client*, Message&&)>;

/**
 * Class with all the code to send and read data to the queue server.
 */
class Client {
 public:
  Client(boost::asio::io_context& io_context, std::string server,
         std::string port);
  /**
   * Send a subscription request to the server.
   *
   * @param topic topic name which the client would like to register to
   * @param callback callback functions called when the client receive a
   * response about the subscription request
   */
  void Subscribe(std::string topic, ReadCallback callback);
  /**
   * Register a callback to messages from the server
   *
   * @param callback Callback function called for any message received
   */
  void Read(ReadCallback callback);
  /**
   * Send a message to the server to create a new topic.
   *
   * @param topic The topic name which the client would like to create
   * @param callback callback function called when the client receive a response
   * about the topic creation request
   */
  void CreateTopic(std::string topic, ReadCallback callback);
  /**
   * Publish a message in a topic
   *
   * @param topic topic name where the given data should be published
   * @param data data to be published
   */
  void PublishMessage(std::string topic, std::string data);

 private:
  /**
   * Write a message in the socket to the server
   *
   * @param message message to be sent
   */
  void Write(Message&& message);
  boost::asio::streambuf _read_buffer;
  boost::asio::streambuf _write_buffer;
  tcp::socket _socket;
  unsigned int _messagesWriteCount;
  unsigned int _messagesReadCount;
};
