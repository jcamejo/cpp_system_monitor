#ifndef SYSTEM_H
#define SYSTEM_H

#include <string>
#include <vector>

#include "os_parser.h"
#include "process.h"
#include "processor.h"

class System {
public:
  Processor &Cpu();
  std::vector<Process> &Processes();
  float MemoryUtilization();
  long UpTime();
  int TotalProcesses();
  int RunningProcesses();
  std::string Kernel();
  std::string OperatingSystem();

  System(OsParser *parser, int totalProcesses = 20) : parser_(parser) {
    totalProcesses_ = totalProcesses < 1 ? 1 : totalProcesses;
    cpu_ = new Processor(parser_);
  };

private:
  std::vector<Process> processes_ = {};
  OsParser *parser_ = nullptr;
  Processor *cpu_;
  int totalProcesses_;
};

#endif