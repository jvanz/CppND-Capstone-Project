#ifndef __SERVER_HPP__
#define __SERVER_HPP__

#include <list>
#include <thread>

#include <boost/asio.hpp>
#include <common/message_queue.hpp>

#include "session.hpp"

using boost::asio::ip::tcp;

/**
 * Class to store all the open connection and wait for more connections
 */
class Server {
 public:
  Server(boost::asio::io_context& io_context, short port);
  ~Server();

 private:
  /**
   * Register the callback for new connection requests
   */
  void Accept();
  /**
   * Start the threads managed by the server
   */
  void StartThreads();

  tcp::acceptor _acceptor;
  std::list<std::shared_ptr<Session>> _sessions;
  std::unique_ptr<MessageQueue<Message>> _pendingMessages;
  std::vector<std::thread> _threads;
};

#endif
