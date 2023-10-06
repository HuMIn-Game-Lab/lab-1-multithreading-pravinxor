#ifndef PARSINGJOB_HPP
#define PARSINGJOB_HPP

#include "job.hpp"

#include <nlohmann/json.hpp>
#include <string>
#include <unordered_map>
#include <vector>

class ParsingJob : public Job<nlohmann::json> {
private:
  std::string ingest;

public:
  ParsingJob(nlohmann::json input);
  nlohmann::json execute() override;
};

#endif
