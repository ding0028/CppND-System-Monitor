#ifndef PROCESSOR_H
#define PROCESSOR_H

class Processor {
 public:
  float Utilization();  // TODO: See src/processor.cpp
  Processor();
  // TODO: Declare any necessary private members
 private:
 float idleTime{0};
 float totalTime{0};
};


#endif