#include "message.hpp"

Message::Message(MessageType type, std::string data)
    : _id{boost::uuids::random_generator()()},
      _type{type},
      _data{data},
      _topic{""} {};

Message::Message(MessageType type, std::string topic, std::string data)
    : _id{boost::uuids::random_generator()()},
      _type{type},
      _data{data},
      _topic{topic} {};

std::ostream &operator<<(std::ostream &output, const Message &msg) {
  boost::archive::text_oarchive oa(output);
  oa << msg;
  output << MESSAGE_TERMINATION_CHAR;
}

std::istream &operator>>(std::istream &input, Message &msg) {
  std::istream::sentry s(input);
  if (s) {
    boost::archive::text_iarchive ia(input);
    ia >> msg;
  }
  return input;
}
