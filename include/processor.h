#ifndef PROCESSOR_H
#define PROCESSOR_H

class Processor {
public:
  float Utilization();

private:
  bool prevSet{false};
  float prevUsage{0.0};
  unsigned int prevTotal{0};
  unsigned int prevIdle{0};
  unsigned int prevNonidle{0};
};

#endif