#ifndef JOB_HPP
#define JOB_HPP

#include <atomic>

class JobSystem;

/// A descriptor for a job to be executed
template <typename T> class Job {

public:
  /// Execute() function, which is ran on Slave threads. This is run first
  virtual T operator()(T) = 0;
};

#endif
