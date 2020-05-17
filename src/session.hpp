#include <boost/asio.hpp>

/**
 * class to control and store the socket for the connected clients
 */
class Session {
 public:
  // TODO RULE OF FIVE
  Session(boost::asio::ip::tcp::socket&& socket);

 private:
  /**
   * Setup the read handler
   */
  void Read();
  boost::asio::ip::tcp::socket _socket;
  constexpr static unsigned int _max_length = 1024;
  char _data[_max_length];
};
