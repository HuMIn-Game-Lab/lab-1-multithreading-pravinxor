#include "makejob.hpp"

#include <array>
#include <cstdio>
#include <iostream>
#include <nlohmann/json_fwd.hpp>
#include <sstream>

MakeJob::MakeJob(nlohmann::json input) { this->target = input["target"]; }

nlohmann::json MakeJob::execute() {
  nlohmann::json status;
  std::string stdout;

  std::string command = "make " + this->target + " 2>&1";
  FILE *pipe = popen(command.c_str(), "r");

  if (pipe) {
    std::array<char, 128> buffer;
    while (fgets(buffer.data(), 128, pipe) != nullptr)
      stdout.append(buffer.data());
  }

  status["stdout"] = stdout;
  status["retcode"] = pclose(pipe);
  return status;
}
