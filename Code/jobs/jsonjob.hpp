#ifndef JSONJOB_HPP
#define JSONJOB_HPP

#include "parsingjob.hpp"

#include <nlohmann/json.hpp>

class JSONJob : public Job<nlohmann::json> {
private:
  nlohmann::json parsed_messages;

public:
  JSONJob(nlohmann::json parsed_messages);
  nlohmann::json execute() override;
};

#endif
