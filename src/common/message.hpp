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

// each message send is terminated with special char
constexpr unsigned char MESSAGE_TERMINATION_CHAR = '\t';

/**
 * All the message type used during communication between the queue system and
 * clients
 */
enum MessageType {
  SUBSCRIBE = 0,       // listen to new message of a topic
  CREATE,              // create a new topic
  SEND,                // send a message to a topic
  TOPIC_NOT_FOUND,     // tell the client that the topic does not exist
  SUBSCRIBED,          //  tell the client it is subscribed to the topic
  TOPIC_CREATED,       //  tell client that topic is successfully created
  CANNOT_CREATE_TOPIC  // tell client topic creation failed
};

/**
 * The base class to represent the message send and receive by the queue system
 * server and its clients.
 */
class Message {
 public:
  Message() : _id{boost::uuids::random_generator()()} {};
  Message(MessageType type, std::string data);
  Message(MessageType type, std::string topic, std::string data);
  Message(const Message &other)  // copy constructor
      : _id(other._id),
        _type(other._type),
        _data(other._data),
        _topic(other._topic){};
  Message(Message &&other) noexcept  // move constructor
  {
    _id = std::move(other._id);
    _type = std::move(other._type);
    _data = std::move(other._data);
    _topic = std::move(other._topic);
  };
  Message &operator=(const Message &other) {
    return *this = Message(other);
  };                                                  // copy assigment
  Message &operator=(const Message &&other) noexcept  // move assigment
  {
    _id = std::move(other._id);
    _type = std::move(other._type);
    _data = std::move(other._data);
    _topic = std::move(other._topic);
    return *this;
  };
  ~Message(){};

  MessageType GetType() const { return _type; };
  std::string GetData() const { return _data; };
  std::string GetTopic() const { return _topic; };
  boost::uuids::uuid GetID() const { return _id; };

  // friends operator used to serialize and deserialize messages
  friend std::ostream &operator<<(std::ostream &output, const Message &msg);
  friend std::istream &operator>>(std::istream &input, Message &msg);
  // Boost serialization library is used to serialize the message sent in the
  // wire
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
 * been created
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
 * Input and output operator to serialize and deserialize message objects
 */
std::ostream &operator<<(std::ostream &output, const Message &msg);
std::istream &operator>>(std::istream &input, Message &msg);

#endif
