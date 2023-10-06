#ifndef JOB_HPP
#define JOB_HPP

#include <atomic>

class JobSystem;

/// The Job interface for which the Job System will execute jobs
class Job {

public:
  /// A unique identifier for the job
  unsigned int id;

  /// Execute() function, which is ran on Slave threads. This is run first
  virtual void execute() = 0;

  /// If a job has dependencies, it may return another job to be immediately
  /// executed (if possible). Notice!!!: Overloading this will likely require
  /// dynamic memory allocation, which should be freed once removed
  /// This is called as soon as execute) is finished (from the Slave thread)
  virtual void chain_next(JobSystem *system){};

  virtual ~Job(){};
};

#endif
