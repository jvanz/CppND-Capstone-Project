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

constexpr unsigned char MESSAGE_TERMINATION_CHAR = '\t';

enum MessageType {
  SUBSCRIBE = 0,       // listen to new message of a topic
  CREATE,              // create a new topic
  SEND,                // send a message to a topic
  TOPIC_NOT_FOUND,     // tell the client that the topic does not exist
  SUBSCRIBED,          //  tell the client it is subscribed to the topic
  TOPIC_CREATED,       //  tell client that topic is successfully created
  CANNOT_CREATE_TOPIC  // tell client topic creation failed
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
 * Sub class to represent a message to the client telling it topic does not
 * exist
 */
class TopicNotFoundMessage : public Message {
 public:
  TopicNotFoundMessage(std::string topic)
      : Message(MessageType::TOPIC_NOT_FOUND, topic, "Topic not found!"){};
};

/**
 * Sub class to represent a message to the client telling it is subscribed to
 * the topic
 * */
class SubscribedMessage : public Message {
 public:
  SubscribedMessage(std::string topic)
      : Message(MessageType::SUBSCRIBED, topic, "Subscribed topic"){};
};

/**
 * Sub class to represent a message to the server telling it which topic the
 * client would like to subscribe to */
class SubscriptionRequestMessage : public Message {
 public:
  SubscriptionRequestMessage(std::string topic)
      : Message(MessageType::SUBSCRIBE, topic, "Subscription request"){};
};

/**
 * Sub class to represent a message to the server telling it to create a
 * new topic */
class CreateTopicMessage : public Message {
 public:
  CreateTopicMessage(std::string topic)
      : Message(MessageType::CREATE, topic, "Create topic"){};
};

/**
 * Sub class to represent a message to the client telling it that the topic has
 * benn created
 */
class TopicCreatedMessage : public Message {
 public:
  TopicCreatedMessage(std::string topic)
      : Message(MessageType::TOPIC_CREATED, topic, "Topic Created"){};
};

/**
 * Sub class to represent a message to the client telling it the server cannot
 * create the topic
 * */
class CannotCreateTopicMessage : public Message {
 public:
  CannotCreateTopicMessage(std::string topic)
      : Message(MessageType::CANNOT_CREATE_TOPIC, topic,
                "Cannot create topic"){};
};

/**
 * Input and outpur operator to serialize and deserialize Message objects
 */
std::ostream &operator<<(std::ostream &output, const Message &msg);
std::istream &operator>>(std::istream &input, Message &msg);

#endif
