#include "job.hpp"
#include "jobsystem.hpp"
#include "makejob.hpp"
#include "parsingjob.hpp"

#include <iostream>
#include <thread>

int main() {
  MakeJob *j = new MakeJob(0, "demo");

  JobSystem js;

  for (unsigned int n = 1; n <= std::thread::hardware_concurrency(); ++n)
    js.add_slave("thread" + std::to_string(n));

  js.enqueue(j);

  std::vector<Job *> completed_jobs = js.get_completed(2);
  // MakeJob *mj = (MakeJob*)completed_jobs[0];
  // std::cerr << mj->stdout << '\n';
  ParsingJob *pj = (ParsingJob *)completed_jobs[1];
  for (Error e : pj->errors) {
    std::cerr << e.column << ' ' << e.line << ' ' << e.filename<< ' ' << e.message << '\n';
  }

  js.cleanup();
  return 0;
}
