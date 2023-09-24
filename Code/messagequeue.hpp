#include <condition_variable>
#include <mutex>
#include <queue>
#include <vector>

template <typename T, template <typename...> class Container, typename C = void>
class MessageQueue {
private:
  /// Lock to manage synchronization between threads internal queue
  std::mutex messages_mtx;

  /// Condition variable that prevents receiving messages until the queue has a
  /// message available
  std::condition_variable cv;

  /// Specializable container, which stores the messages
  Container<T> messages;

public:
  /// Tests whether the queue is empty
  bool empty() {
    std::lock_guard<std::mutex> messages_lock(this->messages_mtx);
    return this->messages.empty();
  }

  /// Adds a message to the queue
  void send(const T &val) {
    std::unique_lock<std::mutex> messages_lock(this->messages_mtx);
    this->messages.push(val);
    messages_lock.unlock();
    this->cv.notify_one();
  }
  /// Receives the next message in the queue. Note: this blocks execution until
  /// a message is available
  T receive() {
    std::unique_lock<std::mutex> messages_lock(this->messages_mtx);
    cv.wait(messages_lock, [this] { return !this->messages.empty(); });

    T val = pop_impl(this->messages);
    return val;
  }

  /// Recieves a specific amount of messages in one call. Note: blocks execution
  /// until n messages are available
  std::vector<T> receive_n(unsigned long n) {
    std::unique_lock<std::mutex> messages_lock(this->messages_mtx);
    cv.wait(messages_lock, [this, n] { return this->messages.size() >= n; });

    std::vector<T> flushed_messages(n);

    for (unsigned long k = 0; k < n; ++k) {
      flushed_messages.push_back(pop_impl(this->messages));
    }

    return flushed_messages;
  }
};

template <typename T> T pop_impl(std::queue<T> &c) {
  T value = c.front();
  c.pop();
  return value;
}

template <typename T, typename C>
class MessageQueue<T, std::priority_queue, C> {
private:
  std::mutex messages_mtx;
  std::condition_variable cv;
  std::priority_queue<T, std::vector<T>, C> messages;

public:
  bool empty() {
    std::lock_guard<std::mutex> messages_lock(this->messages_mtx);
    return this->messages.empty();
  }

  void send(const T &val) {
    std::unique_lock<std::mutex> messages_lock(this->messages_mtx);
    this->messages.push(val);
    messages_lock.unlock();
    this->cv.notify_one(); // Notify any waiting recievers of available element
  }

  T receive() {
    std::unique_lock<std::mutex> messages_lock(this->messages_mtx);
    cv.wait(messages_lock, [this] { return !this->messages.empty(); });

    T val = pop_impl(this->messages);
    return val;
  }

  std::vector<T> receive_n(unsigned long n) {
    std::unique_lock<std::mutex> messages_lock(this->messages_mtx);
    cv.wait(messages_lock, [this, n] { return this->messages.size() >= n; });

    std::vector<T> flushed_messages;

    for (unsigned long k = 0; k < n; ++k) {
      flushed_messages.push_back(pop_impl(this->messages));
    }

    return flushed_messages;
  }
};

template <typename T, typename C>
T pop_impl(std::priority_queue<T, std::vector<T>, C> &c) {
  T value = c.top();
  c.pop();
  return value;
}
