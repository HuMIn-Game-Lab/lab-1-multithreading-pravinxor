/// The Job interface for which the Job System will execute jobs
class Job {
private:
  /// A unique identifier for the job
  int id;
  /// An optional type for the job
  int type;
  /// The channels that this job may run on
  unsigned long channels;

public:
  /// Execute() function, which is ran on Slave threads. This is run first
  virtual void Execute() = 0;

  /// A callback function that is called after Execute() completes on the Master thread. This function should focus on cleanup.
  virtual void JobCompleteCallback() {};
};
