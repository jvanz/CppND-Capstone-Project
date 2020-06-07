# CPPND: Capstone Project Repo

This is the capstone project for the [Udacity C++ Nanodegree Program](https://www.udacity.com/course/c-plus-plus-nanodegree--nd213).
It is a queue system which can be used to pass massages to other processes in a asynchronously fashion. It is inspired
by systems like Apache Kafka and RabbitMQ.

The hole project consist in 3 programs. The queue system itself, producer and consumer program. The producer program
will connect into the queue system to published messages. Those message will be consumed by the consumer program.

The queue system can handle multiples topic at the same time. A topic is a queue where producers can publish their
message to the consumers. Each topic can receive message from multiples producer to multiples consumers consume them.

## Project structure

### Queue system

Overview of the classes and files used in the queue system

#### Server (server.hpp and server.cpp)

This files contain the code of the Server class. This class stores all the open
connection as well as the topics and the methods to process messages and accept
new connections. When a new connection is accepted, a new session object is create
to handle the connection with the client.

#### Session (session.hpp and session.cpp)

The Session class has all the data and methods used to communicate to a client.
It has the code to handle data send by the client as well as the method to write
messages to the client.

Every time the client send a message to the queue system, a task is launched to
process the message and wait before set the callback for new messages again.
The task call the method Server::ProcessPendingMessage.

Every time the client wants to subscribe to a topic, a new thread is launched to
continually follows the message published and send back to the client.

#### Topic (topic.hpp and topic.cpp)

The Topic is a class used to send and get message published in a topic. All the
client subscribed in topics will has a thread constantly calling the method `Receive`
trying to get the message published in the topic. The method `Server::ProcessPendingMessage`
call the `Send` method to publish message into the topic

#### Message queue (message_queue.hpp)

It's a synchronization method under the hood of the Topic class. It guarantees
that only on thread at a time can access a queue with the messages published in
the topics


### Producer and consumer

#### Client (client.hpp and client.cpp)

Client is the base class used by both client program (producer and consumer). It
has all the methods necessary to handle the connection as well as method to send
messages, subscribe to topics, create topic and so on.

#### producer and consumer (producer.cpp and consumer.cpp)

Programs that make use of the Client class to simulate a program sending and receiving
data from the queue system.

### Message (message.hpp and message.cpp)

The Message is a class to ensure the clients program and the queue system will
be able to serialize and deserialize the messages. The Message class and related
files have the code necessary to define and extract info from message exchange
between clients program and server.

## Dependencies

This project requires boost [asio](https://www.boost.org/doc/libs/1_66_0/doc/html/boost_asio.html), [serialization](https://www.boost.org/doc/libs/1_66_0/libs/serialization/doc/index.html)  libraries.

## Basic Build Instructions

If you are in a Ubuntu box, you need to install a more recent Boost Asio version, the version 1.66.0. For that, there
is a target in the Makefile used to install the right boost version. From the root of the project you can run:

```
make boost
```

Once you have the right boost version, you can build the project with the following commands:

```
mkdir build && cd build
cmake .. && make
```

## Running everything

The following example consider you are running all the app in a single machine.

Terminal 1:
```bash
cd build
# Run the queue system spesifiying the listen port as command line argument
./queue 8888
```
During the execution of the queue program, the used will be able to see in the
console all the messages received and sent by the queue system.

Terminal 2:
```
cd build
# Run the producer program with the following arguments, queue system ip, port
# and the topic name where the message will be published
./producer localhost 8888 test
```

During the execution of the program, you will be able to see the ID of the messages
send to the queue system. When all the messages are sent, the program wait for the
user kill the program.

You can do this step multiple time, if you want to

Terminal 3:
```
cd build
# Run the consumer program with the following arguments, queue system ip, port
# and the topic name which the consumer is listen for messages
./consumer localhost 8888 test
```
During the execution of the program, you will be able to see the ID of the messages
published in the topic specified in the command line. If the topic does not exist
yet, the program will keep trying to subscribe to it. Once subscribed, the consumer
will log in the console all the message sent by the queue system, including the
message ID. The program will run until the user kill it.

You can do this step multiple time, if you want to

### Project rubric

#### Loops, Functions, I/O

- The project demonstrates an understanding of C++ functions and control structures:
  server.hpp, server.cpp, session.hpp, session.cpp, topic.hpp, topic.cpp, common/message.hpp, common/message.cpp, message_queue.hpp

- The project reads data from a file and process the data, or the program writes data to a file.
  TODO

- The project accepts user input and processes the input:
  client/producer.cpp line 25, client/consumer.cpp line 52, main.cpp

#### Object Oriented Programming

- The project uses Object Oriented Programming techniques:
  server.hpp, server.cpp, session.hpp, session.cpp, topic.hpp, topic.cpp, common/message.hpp, common/message.cpp, message_queue.hpp

- Classes use appropriate access specifiers for class members:
  server.hpp, server.cpp, session.hpp, session.cpp, topic.hpp, topic.cpp, common/message.hpp, common/message.cpp, message_queue.hpp

- Class constructors utilize member initialization lists:
  sercer.cpp line 8, session.cpp line 10, common/message.cpp line 3

- Classes abstract implementation details from their interfaces:
  server.hpp, server.cpp, session.hpp, session.cpp, topic.hpp, topic.cpp, common/message.hpp, common/message.cpp

- Classes encapsulate behavior.
  server.hpp, server.cpp, session.hpp, session.cpp, topic.hpp, topic.cpp, common/message.hpp, common/message.cpp

- Classes follow an appropriate inheritance hierarchy.
  common/message.hpp, common/message.cpp

- Overloaded functions allow the same function to operate on different parameters.
  session.hpp, session.cpp

- Derived class functions override virtual base class functions:
  TODO

- Templates generalize functions in the project.
  message_queue.hpp

#### Memory management

- The project makes use of references in function declarations.
  server.cpp line 39

- The project uses destructors appropriately:
  server.hpp, session.hpp, topic.hpp, common/message.hpp

- The project uses scope / Resource Acquisition Is Initialization (RAII) where appropriate.
  server.hpp, server.cpp, session.hpp, session.cpp, topic.hpp, topic.cpp, common/message.hpp, common/message.cpp

- The project follows the Rule of 5.
  server.hpp, server.cpp, session.hpp, session.cpp, topic.hpp, topic.cpp, common/message.hpp, common/message.cpp

- The project uses move semantics to move data, instead of copying it, where possible.
  topic.cpp line 21 and 26, server.cpp line 56, client/client.cpp line 13, 32, 40 and 45

- The project uses smart pointers instead of raw pointers.
  server.cpp line 12, 18 and 27

#### Concurrency

- The project uses multithreading:
  session.cpp line 30 and 51

- A promise and future is used in the project:
  session.cpp line 51

- A mutex or lock is used in the project:
  message_queue.hpp, server.cpp

- A condition variable is used in the project:
  message_queue.hpp
