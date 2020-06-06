#ifndef __MESSAGE_QUEUE_HPP__
#define __MESSAGE_QUEUE_HPP__

#include <condition_variable>
#include <mutex>
#include <queue>

/**
 * Message queue to communicate between the threads
 */
template <class T>
class MessageQueue {
 public:
	 // TODO rule of five
  MessageQueue(){};
  MessageQueue(MessageQueue<T> &&other) noexcept {};
  MessageQueue(const MessageQueue<T> &other) noexcept {};
  MessageQueue &operator=(MessageQueue<T> &other) noexcept {};
  MessageQueue &operator=(const MessageQueue<T> &&other) noexcept {};

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
  // TODO needs to be static?
  static std::mutex _mutex;  // cannot access the queue concurrently
  std::condition_variable cv;
  std::queue<T> _queue;  // pending messages
};

template <class T>
std::mutex MessageQueue<T>::_mutex;

#endif
