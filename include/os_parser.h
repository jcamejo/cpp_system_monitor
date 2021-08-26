#ifndef OS_PARSER_H
#define OS_PARSER_H

#include <fstream>
#include <regex>
#include <string>
#include <vector>

using std::string;

class OsParser {
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

  // System
  virtual float MemoryUtilization() const = 0;
  virtual long UpTime() const = 0;
  virtual std::vector<int> Pids() const = 0;
  virtual int TotalProcesses() const = 0;
  virtual int RunningProcesses() const = 0;
  virtual std::string OperatingSystem() const = 0;
  virtual std::string Kernel() const = 0;

  virtual std::vector<std::string> CpuUtilization() const = 0;
  virtual float CpuUtilization(int pid) const = 0;

  // Processes
  virtual std::string Command(int pid) const = 0;
  virtual float Ram(int pid) const = 0;
  virtual std::string Uid(int pid) const = 0;
  virtual std::string User(int pid) const = 0;
  virtual long int UpTime(int pid) const = 0;
};
#endif