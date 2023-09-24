#ifndef JOBSYSTEM_HPP
#define JOBSYSTEM_HPP

#include "slave.hpp"
#include <vector>

enum JobStatus { NEVER_SEEN, QUEUED, RUNNING, COMPLETED, N_JOB_STATUSES };

struct HistoryEntry {
public:
  /// The associated Job ID with the history entry
  int id = 0;
  JobStatus status;
  HistoryEntry(int id = -1, JobStatus status = JobStatus::NEVER_SEEN)
      : id(id), status(status) {}
};

class JobSystem {
  friend Slave;

public:
  /// An atomic (that is only meant to increase), which in theory yields unique
  /// job IDs
  static std::atomic<unsigned long> NEXT_JOB_ID;

  using Message = Job *;

  /// Loads job into queue and adds and entry into the history
  void enqueue(Job *job);

  /// Checks status for specific job id
  JobStatus get_status(unsigned long id) const;

  /// Removes the HistoryEntry (if any), for the specified job ID
  void remove_entry(unsigned long id);

  /// Initializes new thread job system
  void add_slave(std::string name);

  /// Returns a copy of the history entries at the current time
  std::vector<HistoryEntry> current_history();

  /// Joins all threads and prepares the job system for being destroyed
  void cleanup();

private:
  std::vector<Slave> slaves;

  /// *Sends* messages to slaves
  MessageQueue<Slave::Message, std::priority_queue, Slave::CompareMessage>
      queued_jobs;

  /// *Receives* messages from slaves
  MessageQueue<JobSystem::Message, std::queue> completed_jobs;

  /// History entries whose states will be managed by the System and Slaves
  std::vector<HistoryEntry> history;
  std::mutex history_mtx;

  /// Private function to be used by the slaves when jobs are completed
  void update_id_history(Job *job, JobStatus status);

  /// Transitions job from running to complete
  void transition_to_completed(Job *job);
};

#endif
