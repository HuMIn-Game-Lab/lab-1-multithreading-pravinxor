#include <condition_variable>
#include <mutex>
#include <queue>
#include <vector>

template <typename T> class MessageQueue {
private:
  /// Lock to manage synchronization between threads internal queue
  std::mutex messages_mtx;

  /// Condition variable that prevents receiving messages until the queue has a
  /// message available
  std::condition_variable cv;

  std::queue<T> messages;

public:
  /// Tests whether the queue is empty
  bool empty() {
    std::lock_guard<std::mutex> messages_lock(this->messages_mtx);
    return this->messages.empty();
  }

  /// Adds a message to the queue
  void send(const T &val);

  /// Receives the next message in the queue. Note: this blocks execution until
  /// a message is available
  T receive();

  /// Recieves a specific amount of messages in one call
  std::vector<T> receive_n(unsigned long n);
};

template <typename T> void MessageQueue<T>::send(const T &val) {
  std::unique_lock<std::mutex> messages_lock(this->messages_mtx);
  this->messages.push(val);
  messages_lock.unlock();
  this->cv.notify_one(); // Notify any waiting recievers of available element
}

template <typename T> T MessageQueue<T>::receive() {
  std::unique_lock<std::mutex> messages_lock(this->messages_mtx);
  cv.wait(messages_lock, [this] { return !this->messages.empty(); });

  T val = messages.front();
  messages.pop();
  return val;
}

template <typename T>
std::vector<T> MessageQueue<T>::receive_n(unsigned long n) {
  std::unique_lock<std::mutex> messages_lock(this->messages_mtx);
  cv.wait(messages_lock, [this, n] { return this->messages.size() >= n; });

  std::vector<T> flushed_messages = std::vector<T>(n);

  for (unsigned long k = 0; k < n; ++k) {
    flushed_messages.push_back(this->messages.front());
    this->messages.pop();
  }

  return flushed_messages;
}
