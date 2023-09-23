#ifndef MAKEJOB_HPP
#define MAKEJOB_HPP

#include "job.hpp"

#include <string>
class MakeJob : public Job {
public:
  unsigned int id;
  std::string target;
  std::string stdout;
  unsigned char retcode;
  MakeJob(unsigned int id, std::string target);
  void Execute() override;
};

#endif
