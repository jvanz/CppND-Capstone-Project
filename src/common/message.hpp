#ifndef __MESSAGE_HPP__
#define __MESSAGE_HPP__

#include <string>

#include <boost/asio.hpp>
#include <boost/archive/text_oarchive.hpp>
#include <boost/archive/text_iarchive.hpp>

enum MessageType {
  SUBSCRIBE = 0,  // listen to new message of a topic
  CREATE,         // create a new topic
  SEND            // send a message to a topic
};

class Message {
 public:
  // TODO rule of five?
  Message(){};
  Message(MessageType type, std::string data);
  Message(MessageType type, std::string topic, std::string data);
  MessageType GetType() const { return _type; };
  std::string GetData() const { return _data; };
  std::string GetTopic() const { return _topic; };

  friend std::ostream &operator<<(std::ostream &output, const Message &msg);
  friend std::istream &operator>>(std::istream &input, Message &msg);
  friend class boost::serialization::access;
  template<class Archive>
  void serialize(Archive &ar, const unsigned int version)
  {
	  ar & _type;
	  ar & _data;
	  ar & _topic;
  }

 private:
  MessageType _type;
  std::string _data;
  std::string _topic;
};

/**
 * Input and outpur operator to serialize and deserialize Message objects
 */
std::ostream &operator<<(std::ostream &output, const Message &msg);
std::istream &operator>>(std::istream &input, Message &msg);
/**
 * Input and outpur operator to serialize and deserialize MessageType values
 */
std::ostream &operator<<(std::ostream &output, const MessageType &type);
std::istream &operator>>(std::istream &input, MessageType &type);

#endif
