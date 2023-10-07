#ifndef MAKEJOB_HPP
#define MAKEJOB_HPP

#include "job.hpp"

#include <nlohmann/json.hpp>
#include <string>
class MakeJob : public Job<nlohmann::json> {

public:
  nlohmann::json operator()(nlohmann::json target) override;
};

#endif
