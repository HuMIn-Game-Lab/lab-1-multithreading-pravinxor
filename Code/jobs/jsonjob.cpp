#include "jsonjob.hpp"
#include "parsingjob.hpp"

#include <fcntl.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <unistd.h>

JSONJob::JSONJob(nlohmann::json parsed_messages)
    : parsed_messages(parsed_messages) {}

/// Gets the 2 upper and lower surrounding lines as string pairs
std::vector<std::string> get_lines(const std::string &filename,
                                   int lineNumber) {
  // Open the file
  int fd = open(filename.c_str(), O_RDONLY);

  // Get the size of the file
  struct stat sb;
  int ret = fstat(fd, &sb);

  // If the file cannot be opened, return an empty vector
  if (ret == -1) {
    return {};
  }

  // Map the file into memory
  char *file_mapped =
      (char *)mmap(NULL, sb.st_size, PROT_READ, MAP_PRIVATE, fd, 0);

  // If the map failed, return an empty vector
  if (file_mapped == MAP_FAILED) {
    return {};
  }

  std::vector<std::string> result;
  char *start = file_mapped, *end;
  int lineCount = 0;

  for (end = start; end != file_mapped + sb.st_size; ++end) {
    if (*end == '\n') {
      if (lineCount >= lineNumber - 3 && lineCount < lineNumber + 2) {
        std::string line(start, end);
        if (!line.empty())
          result.push_back(line);
      }
      start = end + 1;
      ++lineCount;
    }
  }

  // Add the last line if it doesn't end with a newline and it's in range
  if (end != start && lineCount >= lineNumber - 3 &&
      lineCount < lineNumber + 2) {
    std::string line(start, end);
    if (!line.empty())
      result.push_back(line);
  }

  // Unmap
  munmap(file_mapped, sb.st_size);

  // close file
  close(fd);

  return result;
}

nlohmann::json JSONJob::execute() {
  nlohmann::json output = this->parsed_messages;

  for (nlohmann::json &file_errors : output["compiler"]) {
    for (nlohmann::json &diagnostic : file_errors["diagnostics"]) {
      diagnostic["chunk"] = get_lines(file_errors["filename"], diagnostic["line"]);
    }
  }

  return output;
}
