#include <string>
#include <thread>
#include <variant>

#include "job.hpp"
#include "messagequeue.hpp"

struct NewJob {
  Job *job;
};
struct Exit {};

using SlaveMessage = std::variant<NewJob, Exit>;

class Slave {
private:
  /// A unique name to identify the Slave
  std::string name;

  /// Handle to the thread associated with the slave, which runs work
  /// asynchronously
  std::thread handle;

  /// Message passing queue that stores jobs that are awaiting completion
  MessageQueue<SlaveMessage> *job_waiting_mq;

  /// Daemon that will accept jobs and execute them
  void WorkDaemon();

public:
  Slave(std::string name, MessageQueue<SlaveMessage> *job_waiting_mq);
};
