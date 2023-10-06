#ifndef MAKEJOB_HPP
#define MAKEJOB_HPP

#include "job.hpp"

#include <nlohmann/json.hpp>
#include <string>
class MakeJob : public Job<nlohmann::json> {
private:
  std::string target;

public:
  MakeJob(nlohmann::json input);
  nlohmann::json execute() override;
};

#endif
