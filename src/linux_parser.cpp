#include "linux_parser.h"

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
// DONE: An example of how to read data from the filesystem
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

// https://classroom.udacity.com/nanodegrees/nd213/parts/f9fffe8e-1984-4045-92b6-64854de4df2b/modules/0a4b606e-7aa2-4e1c-8d67-cb5ddd41a6b5/lessons/4636c0ce-e36e-479b-ba36-58b4b849a9e8/concepts/629dbd7b-18f2-4f43-9cb7-d74714b761af
// TODO: Read and return the number of jiffies for the system
long LinuxParser::Jiffies() { return 0; }

// TODO: Read and return the number of active jiffies for a PID
// REMOVE: [[maybe_unused]] once you define the function
long LinuxParser::ActiveJiffies(int pid [[maybe_unused]]) { return 0; }

// TODO: Read and return the number of active jiffies for the system
long LinuxParser::ActiveJiffies() { return 0; }

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
  int value;
  int uid;

  std::ifstream filestream(kProcDirectory + to_string(pid) + kStatFilename);

  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::istringstream linestream(line);
      while (linestream >> label >> value) {
        if (label == "Uid: ") {
          uid = value;
        }
      }
    }
  }

  std::ifstream etcFilestream(kPasswordPath);

  if (etcFilestream.is_open()) {
    while (std::getline(etcFilestream, line)) {
      std::istringstream linestream(line);

      while (linestream >> user) {
        // Use rfind
        // https://www.geeksforgeeks.org/processing-strings-using-stdistringstream/
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
