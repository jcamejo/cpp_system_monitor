#ifndef LINUX_PARSER_H
#define LINUX_PARSER_H
#include "os_parser.h"
class LinuxParser : public IOsParser {
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
}
#endif