#include "jsonjob.hpp"
#include "parsingjob.hpp"
#include <algorithm>
#include <fcntl.h>
#include <fstream>
#include <iostream>
#include <string>
#include <sys/mman.h>
#include <sys/stat.h>
#include <unistd.h>
#include <vector>

JSONJob::JSONJob(
    unsigned int id,
    std::unordered_map<std::string, std::vector<ParsingJob::Error>> *errors)
    : errors(errors) {
  this->id = id;
}

/// Gets the 2 upper and lower surrounding lines as string pairs
std::vector<std::string> getLines(const std::string &filename, int lineNumber) {
  // Open the file
  int fd = open(filename.c_str(), O_RDONLY);

  // Get the size of the file
  struct stat sb;
  fstat(fd, &sb);

  // Map the file into memory
  char *fileInMemory =
      (char *)mmap(NULL, sb.st_size, PROT_READ, MAP_PRIVATE, fd, 0);

  std::vector<std::string> result;
  char *start = fileInMemory, *end;
  int lineCount = 0;

  for (end = start; end != fileInMemory + sb.st_size; ++end) {
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
  munmap(fileInMemory, sb.st_size);

  // close file
  close(fd);

  return result;
}

void JSONJob::execute() {
  for (const std::pair<const std::string, std::vector<ParsingJob::Error>>
           &pair : *this->errors) {
    std::string filename = pair.first;
    for (const ParsingJob::Error &error : pair.second) {
      nlohmann::json entry;
      entry["line"] = error.line;
      entry["column"] = error.column;
      entry["message"] = error.message;
      entry["chunk"] = getLines(filename, error.line);
      this->errors_json[filename].push_back(entry);
    }
  }
}
