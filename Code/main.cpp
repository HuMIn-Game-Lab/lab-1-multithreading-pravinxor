#include "job.hpp"
#include "jobsystem.hpp"
#include "jsonjob.hpp"
#include "makejob.hpp"
#include "parsingjob.hpp"

#include <iostream>
#include <thread>

int main() {
  JobSystem js;
  for (unsigned int n = 1; n <= std::thread::hardware_concurrency(); ++n)
    js.add_slave("thread" + std::to_string(n));

  MakeJob *mj = new MakeJob(0, "demo");
  js.enqueue(mj);

  std::vector<Job *> completed_jobs = js.get_completed(3);
  JSONJob *jj = (JSONJob *)completed_jobs[2];
  std::cout << jj->errors_json;

  for (Job *job : completed_jobs)
    delete job;

  js.join();
  return 0;
}
