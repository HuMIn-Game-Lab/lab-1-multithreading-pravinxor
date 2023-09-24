#include "makejob.hpp"
#include "job.hpp"
#include "jobsystem.hpp"
#include "parsingjob.hpp"

#include <array>
#include <cstdio>
#include <iostream>
#include <sstream>

MakeJob::MakeJob(unsigned int id, std::string target) : id(id), target(target) {
  this->retcode = 0;
}

void MakeJob::execute() {
  std::string command = "make " + this->target;
  std::cerr << "Running: " << command;
  FILE *pipe = popen(command.c_str(), "r");

  if (!pipe) {
    std::cerr << "popen() failed\n";
    this->retcode = 1;
  }

  std::array<char, 128> buffer;
  while (fgets(buffer.data(), 128, pipe) != nullptr) {
    this->stdout.append(buffer.data());
  }
  this->retcode = pclose(pipe);

  std::cout << "Job " << this->id << " has been executed\n";
}

void MakeJob::chain_next(JobSystem *system) {
  ParsingJob *job = new ParsingJob(JobSystem::NEXT_JOB_ID, this->stdout);
  system->enqueue(job);
}
