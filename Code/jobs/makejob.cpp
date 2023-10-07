#include "makejob.hpp"

#include <array>
#include <cstdio>
#include <iostream>
#include <nlohmann/json_fwd.hpp>
#include <sstream>

nlohmann::json MakeJob::operator()(nlohmann::json input) {
  std::string target = input["target"];
  nlohmann::json status;
  std::string stdout;

  std::string command = "make " + target + " 2>&1";
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
