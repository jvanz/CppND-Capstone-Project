# CPPND: Capstone Project Repo

This is the capstone project for the [Udacity C++ Nanodegree Program](https://www.udacity.com/course/c-plus-plus-nanodegree--nd213). 
It is a queue system which can be used to pass massages to other processes in a asynchronously fashion. It is inspired
by systems like Apache Kafka and RabbitMQ.

The hole project consist in 3 programs. The queue system itself, producer and consumer program. The producer program 
will connect into the queue system to published messages. Those message will be consumed by the consumer program. 

The queue system can handle multiples topic at the same time. A topic is a queue where producers can publish their 
message to the consumers. Each topic can receive message from multiples producer to multiples consumers consume them.

## Dependencies

This project requires boost [asio](https://www.boost.org/doc/libs/1_66_0/doc/html/boost_asio.html), [serialization](https://www.boost.org/doc/libs/1_66_0/libs/serialization/doc/index.html),  libraries.

## Basic Build Instructions

```
mkdir build && cd build
cmake .. && make
```

## Running everything

Terminal 1:
```
cd build
./queue
```

Terminal 2:
```
cd build
./producer localhost 8888 test
```
You can do this step multiple time, if you want to

Terminal 3:
```
cd build
./consumer localhost 8888 test
```
You can do this step multiple time, if you want to
