#include "linux_parser.h"
#include "format.h"
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

// What it means is that jiffies is incremented HZ times every second. If HZ =
// 1,000, then it is incremented 1,000 times (that is, one tick every 1/1,000
// seconds). Once defined, the programmable interrupt timer (PIT), which is a
// hardware component, is programmed with that value
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

long LinuxParser::CpuUtilization(int pid) {
  long uptime = LinuxParser::UpTime();
  string line;
  char delim = ' ';
  long utime;     // CPU time spent in user code, measured in clock ticks
  long stime;     // CPU time spent in kernel code, measured in clock ticks
  long cutime;    // Waited-for children's CPU time spent in user code (in
                  // clock ticks)
  long cstime;    // Waited-for children's CPU time spent in kernel code
                  // (in clock ticks)
  long starttime; // Time when the process started, measured in clock ticks
  long totalTime;
  long seconds;
  long usage;
  long hertz = sysconf(_SC_CLK_TCK);

  vector<string> procData{};
  std::ifstream filestream(kProcDirectory + to_string(pid) + kStatFilename);

  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      procData = Helpers::Tokenize(line, delim);

      utime = std::stoul(procData[13]);
      stime = std::stoul(procData[14]);
      cutime = std::stoul(procData[15]);
      cstime = std::stoul(procData[16]);
      starttime = (std::stoul(procData[21]));
    }
  }

  totalTime = utime + stime;
  totalTime += cutime + cstime;
  seconds = uptime - (starttime / hertz);

  if (seconds <= 0) {
    return 0;
  }

  totalTime = totalTime / hertz;
  usage = totalTime / seconds;

  return usage;
}

bool IsNumber(const string &str) {
  for (char const &c : str) {
    if (std::isdigit(c) == 0) {
      return false;
    }
  }

  return true;
}

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

string LinuxParser::Command(int pid) {
  std::ifstream filestream(kProcDirectory + to_string(pid) + kCmdlineFilename);
  string cmdLine;
  string line;

  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::istringstream linestream(line);
      linestream >> cmdLine;
    }
  }

  return cmdLine;
}

string LinuxParser::Ram(int pid) {
  std::ifstream filestream(kProcDirectory + to_string(pid) + kStatusFilename);
  string line;
  string label;
  long value{0};
  long memory{0};

  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::istringstream linestream(line);
      while (linestream >> label >> value) {
        if (label == "VmSize:") {
          memory = value;
          break;
        }
      }
    }
  }

  return to_string(memory / 1000);
}

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

string LinuxParser::User(int pid [[maybe_unused]]) { return string(); }

long LinuxParser::UpTime(int pid) {
  string line;
  char delim = ' ';
  long seconds;
  long hertz = sysconf(_SC_CLK_TCK);
  long starttime;
  vector<string> procData;

  std::ifstream filestream(kProcDirectory + to_string(pid) + kStatFilename);

  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      procData = Helpers::Tokenize(line, delim);
      starttime = (std::stoul(procData[21]));
    }
  }

  seconds = starttime / hertz;

  return seconds;
}
