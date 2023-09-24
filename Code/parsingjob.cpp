#include "parsingjob.hpp"
#include "job.hpp"
#include "jobsystem.hpp"
#include "jsonjob.hpp"

#include <iostream>
#include <regex>
#include <sstream>
#include <string>

ParsingJob::ParsingJob(int id, std::string ingest) : ingest(ingest), id(id) {}

void ParsingJob::execute() {
  std::regex err_expr("(.*):(\\d+):(\\d+): error: (.*)");

  std::istringstream ess(this->ingest);

  std::string line;
  while (std::getline(ess, line)) {
    std::smatch match;
    if (std::regex_match(line, match, err_expr)) {
      // Assumed that errors will have a snippet or extra line, so wait for 2
      // passes to add error
      std::string filename = match[1];

      Error error = {
          .line = std::stoi(match[2]),
          .column = std::stoi(match[3]),
          .message = match[4],
      };
      if (!error.message.empty()) {
        auto error_vec = this->errors.find(filename);
        if (error_vec == this->errors.end()) {
          this->errors.insert({filename, {}});
          error_vec = this->errors.find(filename);
        }
        error_vec->second.push_back(error);
      }
    }
  }
}

void ParsingJob::chain_next(JobSystem *system) {
  JSONJob *job = new JSONJob(JobSystem::NEXT_JOB_ID++, &this->errors);
  system->enqueue(job);
}
