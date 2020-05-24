
#include "message.hpp"

Message::Message(MessageType type, std::string data)
    : _type{type}, _data{data}, _topic{""} {};

Message::Message(MessageType type, std::string topic, std::string data)
    : _type{type}, _data{data}, _topic{topic} {};

std::ostream &operator<<(std::ostream &output, const MessageType &type) {
  unsigned int t = type;
  output << t;
  return output;
}

std::istream &operator>>(std::istream &input, MessageType &type) {
  std::istream::sentry s(input);
  if (s) {
    unsigned int t = -1;
    input >> t;
    type = static_cast<MessageType>(t);
  }
  return input;
}

std::ostream &operator<<(std::ostream &output, const Message &msg) {
  boost::archive::text_oarchive oa(output);
  oa << msg;
}

std::istream &operator>>(std::istream &input, Message &msg) {
  boost::archive::text_iarchive ia(input);
  ia >> msg;
  return input;
}
