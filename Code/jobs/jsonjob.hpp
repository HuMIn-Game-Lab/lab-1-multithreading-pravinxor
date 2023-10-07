#ifndef JSONJOB_HPP
#define JSONJOB_HPP

#include "job.hpp"
#include <nlohmann/json.hpp>

class JSONJob : public Job<nlohmann::json> {
public:
  nlohmann::json operator()(nlohmann::json parsed_messages) override;
};

#endif
