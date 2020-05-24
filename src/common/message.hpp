#ifndef __MESSAGE_HPP__
#define __MESSAGE_HPP__

#include <string>

#include <boost/archive/text_iarchive.hpp>
#include <boost/archive/text_oarchive.hpp>
#include <boost/asio.hpp>
#include <boost/uuid/uuid.hpp>
#include <boost/uuid/uuid_generators.hpp>
#include <boost/uuid/uuid_io.hpp>
#include <boost/uuid/uuid_serialize.hpp>

enum MessageType {
  SUBSCRIBE = 0,  // listen to new message of a topic
  CREATE,         // create a new topic
  SEND            // send a message to a topic
};

class Message {
 public:
  // TODO rule of five?
  Message() : _id{boost::uuids::random_generator()()} {};
  Message(MessageType type, std::string data);
  Message(MessageType type, std::string topic, std::string data);
  MessageType GetType() const { return _type; };
  std::string GetData() const { return _data; };
  std::string GetTopic() const { return _topic; };
  boost::uuids::uuid GetID() const { return _id; };

  friend std::ostream &operator<<(std::ostream &output, const Message &msg);
  friend std::istream &operator>>(std::istream &input, Message &msg);

  friend class boost::serialization::access;
  template <class Archive>
  void serialize(Archive &ar, const unsigned int version) {
    ar &_id;
    ar &_type;
    ar &_data;
    ar &_topic;
  }

 private:
  boost::uuids::uuid _id;
  MessageType _type;
  std::string _data;
  std::string _topic;
};

/**
 * Input and outpur operator to serialize and deserialize Message objects
 */
std::ostream &operator<<(std::ostream &output, const Message &msg);
std::istream &operator>>(std::istream &input, Message &msg);

#endif
