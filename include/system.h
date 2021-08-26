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

  System(OsParser *parser) : parser_(parser) { cpu_ = new Processor(parser_); };

private:
  std::vector<Process> processes_ = {};
  OsParser *parser_ = nullptr;
  Processor *cpu_;
};

#endif