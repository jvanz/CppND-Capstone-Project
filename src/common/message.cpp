
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
  output << msg._type << " ";
  if (msg._type == MessageType::SEND)
    output << msg._topic.size() << " " << msg._topic << " ";
  output << msg._data.size() << " " << msg._data;
  return output;
}

std::istream &operator>>(std::istream &input, Message &msg) {
  std::istream::sentry s(input);
  if (s) {
    input >> msg._type;
    char whitespace;
    input.get(whitespace);
    if (msg._type == MessageType::SEND) {
      std::streamsize topic_length = 0;
      input >> topic_length;
      input.get(whitespace);
      char topic[topic_length];
      input.read(topic, topic_length);
      msg._topic = std::string(topic, topic_length);
    }
    std::streamsize data_length = 0;
    input >> data_length;
    input.get(whitespace);
    char data[data_length];
    input.read(data, data_length);
    msg._data = std::string(data, data_length);
  }
  return input;
}
