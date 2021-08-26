#include <cctype>
#include <sstream>
#include <string>
#include <unistd.h>
#include <vector>

#include "process.h"

using std::string;
using std::to_string;
using std::vector;

int Process::Pid() { return pid_; }

float Process::CpuUtilization() { return parser_->CpuUtilization(pid_); }

string Process::Command() { return parser_->Command(pid_); }

long Process::Ram() const { return parser_->Ram(pid_); }

string Process::User() { return parser_->User(pid_); }

long int Process::UpTime() { return parser_->UpTime(pid_); }

bool Process::operator<(Process const &b) const { return Ram() < b.Ram(); }