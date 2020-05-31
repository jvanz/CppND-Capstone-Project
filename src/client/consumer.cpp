#include <boost/asio.hpp>
#include <iostream>
#include <string>

#include <common/message.hpp>

using boost::asio::ip::tcp;
using namespace std::chrono_literals;

// Forward declarion
class Consumer;
// Callback function used un the Consumer class methods
using ReadCallback = std::function<void(Consumer*, Message&&)>;
// The definiton of the default callbacks for the subscribe and read methods
void defaultReadCallback(Consumer* consumer, Message&& msg);
void defaultSubscribeCallback(Consumer* consumer, Message&& msg);

static unsigned int messagesCount = 0;

class Consumer {
 public:
  Consumer(tcp::socket&& socket) : _socket(std::move(socket)){};

  void Subscribe(ReadCallback callback) {
    std::ostream os(&_write_buffer);
    Message msg(MessageType::SUBSCRIBE, "test");
    os << msg;
    Read(callback);
    boost::asio::write(_socket, _write_buffer);
  };

  void Read(ReadCallback callback) {
    std::cout << "Read()" << std::endl;
    boost::asio::async_read_until(
        _socket, _read_buffer, MESSAGE_TERMINATION_CHAR,
        [this, callback](boost::system::error_code ec, std::size_t length) {
          std::cout << "More data! Error: " << ec << ". Length: " << length
                    << " bytes" << std::endl;
          if (!ec) {
            auto bufs = _read_buffer.data();
            std::istringstream is(
                std::string(boost::asio::buffers_begin(bufs),
                            boost::asio::buffers_begin(bufs) + length));
            Message msg;
            is >> msg;
            _read_buffer.consume(length);
            callback(this, std::move(msg));
          }
        });
  };

 private:
  boost::asio::streambuf _read_buffer;
  boost::asio::streambuf _write_buffer;
  tcp::socket _socket;
};

void defaultReadCallback(Consumer* consumer, Message&& msg) {
  messagesCount += 1;
  std::cout << "Received: " << msg.GetID() << ": " << msg.GetData()
            << " =--> Total: " << messagesCount << std::endl;
  consumer->Read(defaultReadCallback);
};

void defaultSubscribeCallback(Consumer* consumer, Message&& msg) {
  std::cout << "Subscribe callback : " << msg.GetTopic() << " -> "
            << msg.GetData() << "(" << msg.GetID() << ")" << std::endl;
  if (msg.GetType() == MessageType::SUBSCRIBED) {
    consumer->Read(defaultReadCallback);
  } else if (msg.GetType() == MessageType::TOPIC_NOT_FOUND) {
    std::this_thread::sleep_for(1s);
    consumer->Subscribe(defaultSubscribeCallback);
  } else {
    std::cout << "Message type not supported" << std::endl;
  }
};

int main(int argc, char* argv[]) {
  try {
    boost::asio::io_context io_context;
    // prevent the context to stop if there are no events in the loop
    boost::asio::executor_work_guard<boost::asio::io_context::executor_type>
        work = boost::asio::make_work_guard(io_context);

    tcp::resolver resolver(io_context);
    tcp::socket socket(io_context);
    boost::asio::connect(socket, resolver.resolve("localhost", "8888"));

    Consumer consumer(std::move(socket));
    consumer.Subscribe(defaultSubscribeCallback);
    io_context.run();
    std::cout << "Bye!" << std::endl;

  } catch (std::exception& e) {
    std::cerr << "Exception: " << e.what() << "\n";
  }

  return 0;
}
