#ifndef __SESSION_HPP__
#define __SESSION_HPP__

#include <memory>

#include <boost/asio.hpp>

#include "message_queue.hpp"
#include "server.hpp"
#include "topic.hpp"

class Server;
/**
 * class to control and store the socket for the connected clients
 */
class Session {
 public:
  Session(boost::asio::ip::tcp::socket&& socket, Server* server);
  // Disable copy and move. It's not necessary and guarantee no problem with
  // the buffer
  Session(const Session& other) = delete;
  Session& operator=(const Session& other) = delete;
  Session(Session&& other) = delete;
  Session& operator=(Session&& other) = delete;

  /**
   * Make the client listen to a topic
   */
  void Listen(Topic& topic);
  /**
   * Send a message to the client
   */
  void Write(Message&& msg);

 private:
  /**
   * Setup the read handler
   */
  void Read();
  void Write(Message& msg);
  std::unique_ptr<boost::asio::ip::tcp::socket> _socket;
  Server* _server;  // not owner of pointer to the server
  boost::asio::streambuf _buffer;
  std::list<std::thread> _listeningThreads;
};

#endif
