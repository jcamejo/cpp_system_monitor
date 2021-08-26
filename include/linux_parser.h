#ifndef LINUX_PARSER_H
#define LINUX_PARSER_H

#include "os_parser.h"

#include <fstream>
#include <regex>
#include <string>

using std::string;

class LinuxParser : public OsParser {
public:
  // Paths
  const string kProcDirectory{"/proc/"};
  const string kCmdlineFilename{"/cmdline"};
  const string kCpuinfoFilename{"/cpuinfo"};
  const string kStatusFilename{"/status"};
  const string kStatFilename{"/stat"};
  const string kUptimeFilename{"/uptime"};
  const string kMeminfoFilename{"/meminfo"};
  const string kVersionFilename{"/version"};
  const string kOSPath{"/etc/os-release"};
  const string kPasswordPath{"/etc/passwd"};

  enum CPUStates {
    kUser_ = 0,
    kNice_,
    kSystem_,
    kIdle_,
    kIOwait_,
    kIRQ_,
    kSoftIRQ_,
    kSteal_,
    kGuest_,
    kGuestNice_
  };

  // System
  float MemoryUtilization() const override;
  long UpTime() const override;
  std::vector<int> Pids() const override;
  int TotalProcesses() const override;
  int RunningProcesses() const override;
  string OperatingSystem() const override;
  string Kernel() const override;
  std::vector<string> CpuUtilization() const override;
  long CpuUtilization(int pid) const override;

  // Processes
  string Command(int pid) const override;
  long int Ram(int pid) const override;
  string Uid(int pid) const override;
  string User(int pid) const override;
  long UpTime(int pid) const override;
};
#endif