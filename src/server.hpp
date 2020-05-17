#include <boost/asio.hpp>
#include <list>

#include "session.hpp"

using boost::asio::ip::tcp;

/**
 * TODO - documentation
 */
class Server {
 public:
  Server(boost::asio::io_context& io_context, short port);

 private:
  tcp::acceptor _acceptor;
  std::list<std::shared_ptr<Session>> _sessions;
};
