#ifndef __MESSAGE_QUEUE_HPP__
#define __MESSAGE_QUEUE_HPP__

#include <condition_variable>
#include <mutex>
#include <queue>

#include "message.hpp"

/**
 * Message queue to communicate between the threads
 */
template <class T>
class MessageQueue {
 public:
  // TODO rule of five
  MessageQueue(){};

  /**
   * Adds a new item in the queue
   */
  void Send(T &&msg) {
    std::lock_guard<std::mutex> lock(_mutex);
    _queue.push(std::move(msg));
    cv.notify_one();
  };
  /**
   * Remove the first item in the queue
   */
  T Receive() {
    std::unique_lock<std::mutex> lock(_mutex);
    cv.wait(lock, [&] { return _queue.size() > 0; });

    auto next = _queue.front();
    _queue.pop();

    lock.unlock();
    cv.notify_one();
    return next;
  };

 private:
  static std::mutex _mutex;  // cannot access the queue concurrently
  std::condition_variable cv;
  std::queue<T> _queue;  // pending messages
};

template <class T>
std::mutex MessageQueue<T>::_mutex;

#endif
