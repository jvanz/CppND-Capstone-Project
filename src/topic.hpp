#ifndef __TOPIC_HEADER__
#define __TOPIC_HEADER__

#include <iostream>
#include <string>

#include "common/message.hpp"
#include "message_queue.hpp"

/**
 * Class to represent a topic. A topic is a FIFO data stricture where messages
 * can be send. When a message is received, one of the clients listening to
 * the topic will be notified.
 */
class Topic {
 public:
  Topic();
  Topic(std::string name);
  Topic(const Topic &other);             // copy constructor
  Topic &operator=(const Topic &other);  // copy assignment
  Topic(Topic &&other);                  // move constructor
  Topic &operator=(Topic &&other);       // move assignment
  ~Topic() {}

  /**
   * Get the topic name
   */
  std::string GetName() const { return _name; };
  /**
   * Method used to publish a message in the topic
   */
  void Send(Message &&msg);
  /**
   * Get a message published in the topic.
   *
   * Will block until a message is publishes
   */
  Message Receive();

 private:
  std::string _name;
  MessageQueue<Message> _messages;
};

#endif
