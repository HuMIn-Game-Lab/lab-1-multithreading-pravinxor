#ifndef MAKEJOB_HPP
#define MAKEJOB_HPP

#include "job.hpp"

#include <string>
class MakeJob : public Job {
public:
  std::string target;
  std::string stdout;
  unsigned char retcode;
  MakeJob(unsigned int id, std::string target);
  void execute() override;
  void chain_next(JobSystem *system) override;
};

#endif
