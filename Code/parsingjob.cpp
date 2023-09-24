#include "parsingjob.hpp"
#include "job.hpp"
#include <iostream>
#include <regex>
#include <sstream>
#include <string>

ParsingJob::ParsingJob(int id, std::string ingest) : ingest(ingest), id(id) {}

void ParsingJob::execute() {
  Error cur_err;
  std::regex err_expr("(.*):(\\d+):(\\d+): error: (.*)");

  std::istringstream ess(this->ingest);

  std::string line;
  while (std::getline(ess, line)) {
    std::smatch match;
    if (std::regex_match(line, match, err_expr)) {
      if (!cur_err.message.empty())
        this->errors.push_back(cur_err);

      // Assumed that errors will have a snippet or extra line, so wait for 2
      // passes to add error
      cur_err = {
          .filename = match[1],
          .line = std::stoi(match[2]),
          .column = std::stoi(match[3]),
          .message = match[4],
      };
    }
  }
}

void ParsingJob::chain_next(JobSystem *system) {}
