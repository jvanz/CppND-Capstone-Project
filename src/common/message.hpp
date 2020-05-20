#ifndef __MESSAGE_HPP__
#define __MESSAGE_HPP__

#include <string>

#include <boost/asio.hpp>

enum MessageType { SUBSCRIBE = 0, SEND };

class Message {
 public:
  // TODO rule of five?
  Message(){};
  Message(MessageType type, std::string data);
  MessageType GetType() const { return _type; };
  std::string GetData() const { return _data; };

  friend std::ostream &operator<<(std::ostream &output, const Message &msg);
  friend std::istream &operator>>(std::istream &input, Message &msg);

 private:
  MessageType _type;
  std::string _data;
};

std::ostream &operator<<(std::ostream &output, const Message &msg);
std::istream &operator>>(std::istream &input, Message &msg);

#endif
