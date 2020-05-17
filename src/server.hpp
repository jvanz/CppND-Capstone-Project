#include <boost/asio.hpp>

using boost::asio::ip::tcp;

/**
 * TODO - documentation
 */
class Server {
 public:
  Server(boost::asio::io_context& io_context, short port);

 private:
  tcp::acceptor _acceptor;
};
