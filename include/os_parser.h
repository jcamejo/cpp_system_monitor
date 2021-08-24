#ifndef OS_PARSER_H
#define OS_PARSER_H

// TODO: OS PARSER COMPLETE Refactor
class OsParser {
public:
  // System
  virtual float MemoryUtilization() const = 0;
  virtual long UpTime() const = 0;
  virtual std::vector<int> Pids() const = 0;
  virtual int TotalProcesses() const = 0;
  virtual int RunningProcesses() const = 0;
  virtual std::string OperatingSystem() const = 0;
  virtual std::string Kernel() const = 0;

  virtual std::vector<std::string> CpuUtilization() const = 0;
  virtual long CpuUtilization(int pid) const = 0;

  // Processes
  virtual std::string Command(int pid) const = 0;
  virtual long int Ram(int pid) const = 0;
  virtual std::string Uid(int pid) const = 0;
  virtual std::string User(int pid) const = 0;
  virtual long int UpTime(int pid) const = 0;
} // namespace OsParser
#endif