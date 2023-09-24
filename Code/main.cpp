#include "job.hpp"
#include "jobsystem.hpp"
#include "jsonjob.hpp"
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

  std::vector<Job *> completed_jobs = js.get_completed(3);
  // MakeJob *mj = (MakeJob*)completed_jobs[0];
  // std::cerr << mj->stdout << '\n';
  JSONJob *jj   = (JSONJob*)completed_jobs[2];
  std::cout << jj->errors_json;
  // for (auto t : *pj->errors) {
  //   std::cerr << "In file: " << t.first << '\n';
  //   std::cerr << t.second.column << ' ' << t.second.line << ' '
  //             << t.second.filename << ' ' << t.second.message << '\n';
  // }

  js.cleanup();
  return 0;
}
