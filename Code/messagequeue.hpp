#include <condition_variable>
#include <mutex>
#include <queue>

template <typename T> class MessageQueue {
private:
  /// Lock to manage synchronization between threads internal queue
  std::mutex mtx;

  /// Condition variable that prevents receiving messages until the queue has a
  /// message available
  std::condition_variable cv;

  std::queue<T> messages;

public:
  /// Adds a message to the queue
  void send(const T &val);

  /// Receives the next message in the queue. Note: this blocks execution until
  /// a message is available
  T receive();
};

template <typename T> void MessageQueue<T>::send(const T &val) {
  std::lock_guard<std::mutex> lock(this->mtx);
  this->messages.push(val);

  this->cv.notify_one(); // Notify any waiting recievers of available element
}

template <typename T> T MessageQueue<T>::receive() {
  std::unique_lock<std::mutex> lock(this->mtx);
  cv.wait(lock, [this] { return !messages.empty(); });

  T val = messages.back();
  messages.pop();
  return val;
}
