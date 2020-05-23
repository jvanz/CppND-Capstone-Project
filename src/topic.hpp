#ifndef __TOPIC_HEADER__
#define __TOPIC_HEADER__

#include <iostream>
#include <string>

#include "common/message_queue.hpp"
/**
 * Class to represent a topic. A topic is a FIFO data strcuture where messages
 * can be send. When a message is recieved, on of the clients listening to
 * the topic will receive a message.
 */
class Topic {
 public:
  Topic();
  Topic(std::string name);
  Topic(const Topic &other);
  Topic &operator=(const Topic &other);
  Topic(Topic &&other);
  Topic &operator=(Topic &&other);

  std::string GetName() const { return _name; };
  void Send(Message &&msg);
  Message Receive();

 private:
  std::string _name;
  MessageQueue<Message> _messages;
};

#endif
