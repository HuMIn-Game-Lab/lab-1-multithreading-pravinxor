#ifndef PARSINGJOB_HPP
#define PARSINGJOB_HPP

#include "job.hpp"
#include <string>
#include <unordered_map>
#include <vector>

class ParsingJob : public Job {
private:
  std::string ingest;

public:
  struct Error {
    std::string filename;
    int line, column;
    std::string message;
  };

  /// Errors mapped to the filenames where they occured
  std::unordered_map<std::string, std::vector<ParsingJob::Error>> errors;
  unsigned int id;
  ParsingJob(int id, std::string ingest);
  void execute() override;
  void chain_next(JobSystem *system) override;
};

#endif
