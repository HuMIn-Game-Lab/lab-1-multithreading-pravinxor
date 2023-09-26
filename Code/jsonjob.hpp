#ifndef JSONJOB_HPP
#define JSONJOB_HPP

#include "job.hpp"
#include "parsingjob.hpp"

#include <nlohmann/json.hpp>
#include <nlohmann/json_fwd.hpp>

class JSONJob : public Job {
private:
  std::unordered_map<std::string, std::vector<ParsingJob::Error>> *errors;

public:
  nlohmann::json errors_json;
  JSONJob(
      unsigned int id,
      std::unordered_map<std::string, std::vector<ParsingJob::Error>> *errors);
  void execute() override;
};

#endif
