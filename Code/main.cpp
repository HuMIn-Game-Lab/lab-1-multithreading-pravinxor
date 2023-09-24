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
  MakeJob *j = new MakeJob(0, "demo");
  js.enqueue(j);
  js.cleanup();
  std::vector<HistoryEntry> entries = js.current_history();

  for (HistoryEntry entry : entries) {
    std::cout << entry.id << ' ' << entry.status << '\n';
  }

  std::cout << j->stdout << '\n';

  delete j;
  return 0;
}
