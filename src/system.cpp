#include "system.h"

#include <unistd.h>

#include <cstddef>
#include <set>
#include <string>
#include <vector>

#include "helpers.h"
#include "linux_parser.h"
#include "process.h"

#include "processor.h"

using std::set;
using std::size_t;
using std::string;
using std::vector;

Processor &System::Cpu() { return *cpu_; }

vector<Process> &System::Processes() {
  vector<int> pids = parser_->Pids();
  processes_ = {};

  for (int pid : pids) {
    Process p(pid, parser_);
    processes_.push_back(p);
  }

  std::sort(processes_.begin(), processes_.end(),
            [](const Process &a, const Process &b) { return b < a; });

  vector<Process>::const_iterator first = processes_.begin();
  vector<Process>::const_iterator last = processes_.begin() + totalProcesses_;
  vector<Process> *procs = new vector<Process>(first, last);
  return *procs;
}

// OS
std::string System::Kernel() { return parser_->Kernel(); }

float System::MemoryUtilization() { return parser_->MemoryUtilization(); }

std::string System::OperatingSystem() { return parser_->OperatingSystem(); }

int System::RunningProcesses() { return parser_->RunningProcesses(); }

int System::TotalProcesses() { return parser_->TotalProcesses(); }

long int System::UpTime() { return parser_->UpTime(); }