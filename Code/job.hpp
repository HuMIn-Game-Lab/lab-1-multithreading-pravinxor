#ifndef JOB_HPP
#define JOB_HPP

/// The Job interface for which the Job System will execute jobs
class Job {
public:
  /// A unique identifier for the job
  int id;

  /// Execute() function, which is ran on Slave threads. This is run first
  virtual void Execute() = 0;

  /// A callback function that is called after Execute() completes on the Master
  /// thread. This function should focus on cleanup/lightweight tasks.
  virtual void JobCompleteCallback(){};
};

#endif
