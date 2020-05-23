#include <iostream>

#include "topic.hpp"

Topic::Topic() : _name{""} {};

Topic::Topic(std::string name) : _name{name} {
  std::cout << "New topic: " << _name << std::endl;
};

Topic::Topic(const Topic &other) : _name{other._name} {};

Topic::Topic(Topic &&other) { _name = std::move(other._name); }

Topic &Topic::operator=(const Topic &other) {
  _name = other._name;
  return *this;
}

Topic &Topic::operator=(Topic &&other) {
  _name = std::move(other._name);
  return *this;
}

void Topic::Send(Message &&msg) {
  _messages.Send(std::move(msg));
  std::cout << "Message published" << std::endl;
};

Message Topic::Receive() { return _messages.Receive(); };
