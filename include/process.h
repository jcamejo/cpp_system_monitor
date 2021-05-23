#ifndef PROCESS_H
#define PROCESS_H

#include <string>
/*
Basic class for Process representation
It contains relevant attributes as shown below
*/

using std::string;

class Process {
public:
  int Pid();                              // TODO: See src/process.cpp
  std::string User();                     // TODO: See src/process.cpp
  std::string Command();                  // TODO: See src/process.cpp
  float CpuUtilization();                 // TODO: See src/process.cpp
  std::string Ram();                      // TODO: See src/process.cpp
  long int UpTime();                      // TODO: See src/process.cpp
  bool operator<(Process const &a) const; // TODO: See src/process.cpp

  Process(int pid, string basePath) : pid_(pid) {
    path_ = basePath + "/" + std::to_string(pid_);
    cpu_usage = CpuUtilization();
  };

  float cpu_usage;

private:
  int pid_;
  std::string path_;
};

#endif