#ifndef PROCESS_H
#define PROCESS_H
#include "os_parser.h"

#include <string>
/*
Basic class for Process representation
It contains relevant attributes as shown below
*/

using std::string;

class Process {
public:
  int Pid();
  std::string User();
  std::string Command();
  float CpuUtilization();
  long Ram() const;
  long int UpTime();
  bool operator<(Process const &a) const;

  Process(int pid, OsParser *parser) : pid_(pid), parser_(parser) {
    path_ = parser->kProcDirectory + std::to_string(pid_);
  };

  float cpu_usage;
  long int memory{0};

private:
  int pid_;
  OsParser *parser_;
  std::string path_;
};

#endif