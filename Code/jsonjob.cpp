#include "jsonjob.hpp"
#include "parsingjob.hpp"
#include <iostream>

JSONJob::JSONJob(
    int id,
    std::unordered_map<std::string, std::vector<ParsingJob::Error>> *errors)
    : errors(errors), id(id) {}

void JSONJob::execute() {
  for (const std::pair<const std::string, std::vector<ParsingJob::Error>>
           &pair : *this->errors) {
    std::string filename = pair.first;
    for (const ParsingJob::Error &error : pair.second) {
      nlohmann::json entry;
      entry["line"] = error.line;
      entry["column"] = error.column;
      entry["message"] = error.message;
      this->errors_json[filename].push_back(entry);
    }
  }
}
