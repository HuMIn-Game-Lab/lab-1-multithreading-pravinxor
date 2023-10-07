#include "parsingjob.hpp"

#include <iostream>
#include <regex>
#include <sstream>
#include <string>
#include <vector>

const std::regex
    linker_txt_expr("\\(.\\w+\\+0x\\w+\\): undefined reference to `\\w+'");
const std::regex linker_expr("clang-\\d+: error: (.*)");
const std::regex compiler_expr("(.*):(\\d+):(\\d+): (?:error|warning): (.*)");

void add_error_to_file(nlohmann::json &output, const std::string &filename,
                       const nlohmann::json &diagnostic) {
  // Searching for the filename entry
  auto it = std::find_if(output.begin(), output.end(),
                         [filename](const nlohmann::json &obj) {
                           return obj["filename"] == filename;
                         });

  if (it == output.end()) {
    output.push_back({{"filename", filename}, {"diagnostics", {diagnostic}}});
  } else {
    (*it)["diagnostics"].push_back(diagnostic);
  }
}

nlohmann::json ParsingJob::operator()(nlohmann::json ingest) {
  nlohmann::json output;

  std::string stdout = ingest["stdout"];

  std::string line;
  std::string linker_files;

  std::istringstream ess(stdout);
  while (std::getline(ess, line)) {
    std::smatch match;
    if (std::regex_match(line, match, compiler_expr)) {
      // Assumed that errors will have a snippet or extra line, so wait for 2
      // passes to add error
      std::string filename = match[1];
      nlohmann::json diagnostic = {{"line", std::stoi(match[2])},
                                   {"column", std::stoi(match[3])},
                                   {"message", match[4]}};
      add_error_to_file(output["compiler"], filename, diagnostic);
    } else if (std::regex_match(line, match, linker_txt_expr)) {
      linker_files.append(line + '\n');
    } else if (std::regex_match(line, match, linker_expr)) {
      output["linker"] = {{"functions", linker_files},
                          {
                              {"error", match[1]},
                          }};
    }
  }
  return output;
}
