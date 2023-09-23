#include "job.hpp"
#include "jobsystem.hpp"
#include "makejob.hpp"

#include <iostream>
#include <thread>

MessageQueue<int> mq;

int main() {
  JobSystem js;
  js.add_slave("thread1");
  js.add_slave("thread2");
  std::string target = "demo";
  Job *j = new MakeJob(0, target);
  js.enqueue(j);
  js.cleanup();
  delete j;
  return 0;
}
