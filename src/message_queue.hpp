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
  MessageQueue(){};
  MessageQueue(MessageQueue<T> &&other) noexcept {
    _mutex = std::move(other._mutex);
    _cv = std::move(other._cv);
    _queue = std::move(other._queue);
  };
  MessageQueue &operator=(MessageQueue<T> &&other) noexcept {
    _mutex = std::move(other._mutex);
    _cv = std::move(other._cv);
    _queue = std::move(other._queue);
    return *this;
  };
  ~MessageQueue(){};
  // Cannot be copied
  MessageQueue(const MessageQueue<T> &other) = delete;  //
  MessageQueue &operator=(MessageQueue<T> &other) = delete;

  /**
   * Adds a new item in the queue
   */
  void Send(T &&msg) {
    std::lock_guard<std::mutex> lock(_mutex);
    _queue.push(std::move(msg));
    _cv.notify_all();
  };
  /**
   * Remove the first item in the queue
   */
  T Receive() {
    std::unique_lock<std::mutex> lock(_mutex);
    _cv.wait(lock, [&] { return _queue.size() > 0; });

    auto next = _queue.front();
    _queue.pop();

    lock.unlock();
    _cv.notify_one();
    return next;
  };

 private:
  std::mutex _mutex;  // cannot access the queue concurrently
  std::condition_variable _cv;
  std::queue<T> _queue;  // pending messages
};

#endif
