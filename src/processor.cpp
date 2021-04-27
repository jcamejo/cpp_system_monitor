#include "processor.h"

#include <iostream>
#include <sstream>
#include <string>
#include <vector>

#include "linux_parser.h"

using std::string;
using std::vector;

// TODO: Return the aggregate CPU utilization
float Processor::Utilization() {
  vector<string> cpus;
  vector<int> usage;
  int value;
  string cpu;
  string cpuName;

  cpus = LinuxParser::CpuUtilization();

  // using only one CPU
  if (cpus.size() >= 1) {
    cpu = cpus[0];
    std::istringstream linestream(cpu);

    linestream >> cpuName;

    while (linestream >> value) {
      usage.push_back(value);
    }
  }

  return 0.0;
}