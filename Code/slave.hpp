#ifndef SLAVE_HPP
#define SLAVE_HPP

#include <string>
#include <thread>
#include <variant>

#include "job.hpp"
#include "messagequeue.hpp"

// Forward declaration to avoid circular dependency
class JobSystem;

class Slave {
  friend JobSystem;

public:
  struct NewJob {
    Job *job;
  };
  struct Exit {};

  using Message = std::variant<NewJob, Exit>;

  Slave(std::string name, MessageQueue<Slave::Message> *job_waiting_mq,
        JobSystem *system);

private:
  /// A unique name to identify the Slave
  std::string name;

  /// Handle to the thread associated with the slave, which runs work
  /// asynchronously
  std::thread handle;

  /// Reference to System to perform post-completion tasks
  JobSystem *system;

  /// Daemon that will accept jobs and execute them
  void WorkDaemon();
};

#endif
