#ifndef PARSINGJOB_HPP
#define PARSINGJOB_HPP

#include "job.hpp"
#include <string>
#include <vector>

struct Error {
  std::string filename;
  int line, column;
  std::string message;
};

class ParsingJob : public Job {
private:
  std::string ingest;

public:
  std::vector<Error> errors;
  unsigned int id;
  ParsingJob(int id, std::string ingest);
  void execute() override;
  void chain_next(JobSystem *system) override;
};

#endif
