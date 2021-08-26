#ifndef PROCESSOR_H
#define PROCESSOR_H

#include "os_parser.h"

class Processor {
public:
  float Utilization();
  Processor(OsParser *parser) : parser_(parser){};

private:
  bool prevSet{false};
  float prevUsage{0.0};
  unsigned int prevTotal{0};
  unsigned int prevIdle{0};
  unsigned int prevNonidle{0};
  OsParser *parser_;
};

#endif