#ifndef __SESSION_HPP__
#define __SESSION_HPP__

#include <memory>

#include <boost/asio.hpp>

#include <common/message_queue.hpp>
#include "server.hpp"

class Server;
/**
 * class to control and store the socket for the connected clients
 */
class Session {
 public:
  Session(boost::asio::ip::tcp::socket&& socket, Server* server);

  // TODO RULE OF FIVE
  Session(Session&& other);             // move constructor
  Session& operator=(Session&& other);  // move assignment
  // Disable copy. Socket is not copyable
  Session(const Session& other) = delete;             // copy constructor
  Session& operator=(const Session& other) = delete;  // copy assignment

 private:
  /**
   * Setup the read handler
   */
  void Read();
  std::unique_ptr<boost::asio::ip::tcp::socket> _socket;
  Server* _server;
  boost::asio::streambuf _buffer;
};

#endif
