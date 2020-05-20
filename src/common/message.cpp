#include "message.hpp"

Message::Message(MessageType type, std::string data)
    : _type{type}, _data{data} {};

std::ostream &operator<<(std::ostream &output, const Message &msg) {
  output << msg._type << msg._data;
  return output;
}

std::istream &operator>>(std::istream &input, Message &msg) {
  std::istream::sentry s(input);
  if (s) {
    int type = -1;
    input >> std::noskipws >> type >> msg._data >> std::skipws;
    msg._type = static_cast<MessageType>(type);
  }
  return input;
}
