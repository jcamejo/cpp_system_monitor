#include "linux_parser.h"
#include "helpers.h"

#include <dirent.h>
#include <unistd.h>

#include <filesystem>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

using std::cout;
using std::stof;
using std::string;
using std::to_string;
using std::vector;

namespace fs = std::filesystem;

// Idea: Making the Parser adaptable to not only the Linux Parser but also other
string LinuxParser::OperatingSystem() {
  string line;
  string key;
  string value;
  std::ifstream filestream(kOSPath);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::replace(line.begin(), line.end(), ' ', '_');
      std::replace(line.begin(), line.end(), '=', ' ');
      std::replace(line.begin(), line.end(), '"', ' ');
      std::istringstream linestream(line);

      while (linestream >> key >> value) {
        if (key == "PRETTY_NAME") {
          std::replace(value.begin(), value.end(), '_', ' ');
          return value;
        }
      }
    }
  }
  return value;
}

string LinuxParser::Kernel() {
  string os, kernel, version;
  string line;
  std::ifstream stream(kProcDirectory + kVersionFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    linestream >> os >> version >> kernel;
  }
  return kernel;
}

vector<int> LinuxParser::Pids() {
  vector<int> pids;

  for (auto &dir : fs::directory_iterator(kProcDirectory)) {
    string dirname = dir.path().stem().string();
    if (std::all_of(dirname.begin(), dirname.end(), isdigit)) {
      int pid = std::stoi(dirname);
      pids.push_back(pid);
    }
  }

  return pids;
}

float LinuxParser::MemoryUtilization() {
  string line;
  string label;
  float value;
  float memTotal{0};
  float memFree{0};
  float memUsed{0};

  std::ifstream filestream(kProcDirectory + kMeminfoFilename);

  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::istringstream linestream(line);

      while (linestream >> label >> value) {
        if (label == "MemTotal:") {
          memTotal = value;
        }

        if (label == "MemFree:") {
          memFree = value;
        }
      }
    }
  }

  memUsed = memTotal - memFree;

  return memUsed / memTotal;
}

// TODO: Read and return the system uptime
long LinuxParser::UpTime() {
  string line;
  string label;
  long uptime;
  long idleUptime;

  std::ifstream filestream(kProcDirectory + kUptimeFilename);

  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::istringstream linestream(line);

      while (linestream >> uptime >> idleUptime) {
      }
    }
  }

  return uptime;
}

// TODO: Read and return the number of jiffies for the system

// What it means is that jiffies is incremented HZ times every second. If HZ =
// 1,000, then it is incremented 1,000 times (that is, one tick every 1/1,000
// seconds). Once defined, the programmable interrupt timer (PIT), which is a
// hardware component, is programmed with that value
long LinuxParser::Jiffies() { return 0; }

long LinuxParser::ActiveJiffies(int pid) {}

// TODO: Read and return the number of active jiffies for the system
long LinuxParser::ActiveJiffies() {
  return 0;

  // TODO: Read and return the number of idle jiffies for the system
  long LinuxParser::IdleJiffies() { return 0; }

  // TODO: Read and return CPU utilization
  vector<string> LinuxParser::CpuUtilization() {
    string line;
    vector<string> cpus;
    std::ifstream filestream(kProcDirectory + kStatFilename);

    if (filestream.is_open()) {
      while (std::getline(filestream, line)) {
        if (line.substr(0, 3) == "cpu") {
          cpus.push_back(line);
        }
      }
    }

    return cpus;
  }

  // TODO::Read and return CPU utilization of a specific PID
  long LinuxParser::CpuUtilization(int pid) {
    long uptime = LinuxParser::Uptime();
    string line;
    char delim = ' ';
    long utime;  // CPU time spent in user code, measured in clock ticks
    long stime;  // CPU time spent in kernel code, measured in clock ticks
    long cutime; // Waited-for children's CPU time spent in user code (in clock
                 // ticks)
    long cstime; // Waited-for children's CPU time spent in kernel code (in
                 // clock ticks)
    long starttime; // Time when the process started, measured in clock ticks
    long hertz = sysconf(_SC_CLK_TCK);

    vector<string> procData{};
    std::ifstream filestream(kProcDirectory + to_string(pid) + kStatFilename);

    if (filestream.is_open()) {
      whie(std::getline(filestream, line)) {
        procData = Helpers::Tokenize(line, delim);
        utime = std::stoi(procData[14]);
        stime = std::stoi(procData[15]);
        cutime = std::stoi(procData[16]);
        cstime = std::stoi(procData[17]);
        starttime = std::stoi(procData[22]);
      }
    }

    return 0;
  }

  bool IsNumber(const string &str) {
    for (char const &c : str) {
      if (std::isdigit(c) == 0) {
        return false;
      }
    }

    return true;
  }

  // TODO: Read and return the total number of processes
  int LinuxParser::TotalProcesses() {
    string line;
    string label;
    int value;
    int total{0};

    std::ifstream filestream(kProcDirectory + kStatFilename);

    if (filestream.is_open()) {
      while (std::getline(filestream, line)) {
        std::istringstream linestream(line);

        while (linestream >> label >> value) {
          if (label == "processes") {
            total = value;
          }
        }
      }
    }

    return total;
  }

  // TODO: Read and return the number of running processes
  int LinuxParser::RunningProcesses() {
    string line;
    string label;
    int value;
    int total{0};

    std::ifstream filestream(kProcDirectory + kStatFilename);

    if (filestream.is_open()) {
      while (std::getline(filestream, line)) {
        std::istringstream linestream(line);

        while (linestream >> label >> value) {
          if (label == "procs_running") {
            total = value;
          }
        }
      }
    }

    return total;
  }

  // TODO: Read and return the command associated with a process
  // REMOVE: [[maybe_unused]] once you define the function
  string LinuxParser::Command(int pid [[maybe_unused]]) { return string(); }

  // TODO: Read and return the memory used by a process
  // REMOVE: [[maybe_unused]] once you define the function
  string LinuxParser::Ram(int pid [[maybe_unused]]) { return string(); }

  string LinuxParser::Uid(int pid) {
    string label;
    string line;
    string username;
    string user;
    string userId;
    vector<string> userData;
    string uid;
    string value;

    string statPath = kProcDirectory + to_string(pid) + kStatusFilename;

    std::ifstream filestream(statPath);

    if (filestream.is_open()) {
      while (std::getline(filestream, line)) {
        std::istringstream linestream(line);
        while (linestream >> label >> value) {
          if (label == "Uid:") {
            uid = value;
          }
        }
      }
    }

    std::ifstream etcFilestream(kPasswordPath);

    if (etcFilestream.is_open()) {
      while (std::getline(etcFilestream, line)) {
        char delim = ':';
        userData = Helpers::Tokenize(line, delim);

        if (uid == userData[2]) {
          username = userData[0];
        }
      }
    }

    return username;
  }

  // TODO: Read and return the user associated with a process
  // REMOVE: [[maybe_unused]] once you define the function
  string LinuxParser::User(int pid [[maybe_unused]]) { return string(); }

  // TODO: Read and return the uptime of a process
  // REMOVE: [[maybe_unused]] once you define the function
  long LinuxParser::UpTime(int pid [[maybe_unused]]) { return 0; }
