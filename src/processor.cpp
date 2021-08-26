#include "processor.h"

#include <iostream>
#include <sstream>
#include <string>
#include <vector>

#include "linux_parser.h"

using std::string;
using std::vector;

/*
* Used formula
* https://stackoverflow.com/questions/23367857/accurate-calculation-of-cpu-usage-given-in-percentage-in-linux
* PrevIdle = previdle + previowait
* Idle = idle + iowait

* PrevNonIdle = prevuser + prevnice + prevsystem + previrq + prevsoftirq +
* prevsteal NonIdle = user + nice + system + irq + softirq + steal

* PrevTotal = PrevIdle + PrevNonIdle
* Total = Idle + NonIdle

* # differentiate: actual value minus the previous one
* totald = Total - PrevTotal
* idled = Idle - PrevIdle

* CPU_Percentage = (totald - idled)/totald
*/

float Processor::Utilization() {
  vector<string> cpus;
  vector<int> usage;
  int value;
  string cpu;
  string cpuName;

  // Work times
  unsigned int user{0};
  unsigned int nice{0};
  unsigned int system{0};
  unsigned int idle{0};
  unsigned int iowait{0};
  unsigned int irq{0};
  unsigned int softirq{0};
  unsigned int steal{0};
  unsigned int guest{0};
  unsigned int guestNice{0};
  unsigned int idled{0};
  unsigned int nonIdled{0};
  unsigned int total{0};

  float used{0};

  cpus = parser_->CpuUtilization();

  // using only one CPU
  if (cpus.size() >= 1) {
    cpu = cpus[0];
    std::istringstream linestream(cpu);

    linestream >> cpuName;

    while (linestream >> value) {
      usage.push_back(value);
    }
  } else {
    return 0.0;
  }

  // TODO: Change to Enums
  user = usage[0];
  nice = usage[1];
  system = usage[2];
  idle = usage[3];
  iowait = usage[4];
  irq = usage[5];
  softirq = usage[6];
  steal = usage[7];
  guest = usage[8];
  guestNice = usage[9];

  idled = idle + iowait;
  nonIdled = user + nice + system + irq + softirq + steal;
  total = idled + nonIdled;

  if (total == 0) {
    return 0;
  }

  total = total - prevTotal;
  idled = idled - prevIdle;
  used = (float)(total - idled) / (float)total;

  if (!prevSet) {
    prevIdle = idled;
    prevNonidle = nonIdled;
    prevTotal = total;
  }

  prevSet = true;
  return used;
}