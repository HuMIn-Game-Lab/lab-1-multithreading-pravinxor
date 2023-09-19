#include <mutex>
#include <queue>

template <typename T> class MessageQueue {
private:
  /// Lock that is applied when there are no messages in queue
  std::recursive_mutex emptylock;

  /// Lock to manage synchronization between thread and messagequeue
  std::mutex msglock;

  std::queue<T> messages;

public:
  /// Adds a message to the queue
  void send(const T &val);

  /// Receives the next message in the queue. Note: this blocks execution until
  /// a message is available
  T &receive();
};

template <typename T> void MessageQueue<T>::send(const T &val) {
  this->msglock.unlock();
  this->messages.push(val);
  this->msglock.lock();
}

template <typename T> T &MessageQueue<T>::receive() {
  this->emptylock.lock();

  this->msglock.lock();
  T val = messages.back();
  messages.pop();
  this->msglock.unlock();

  if (!messages.empty()) {
    this->emptylock.unlock();
  }

  return val;
}
