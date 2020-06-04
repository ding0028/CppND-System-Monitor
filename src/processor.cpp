#include "processor.h"
#include "linux_parser.h"
// TODO: Return the aggregate CPU utilization
float Processor::Utilization() {
  
  float idleTime = LinuxParser::IdleJiffies();  
  float totalTime = LinuxParser::Jiffies();
  if (totalTime <= 0)
    return 0;
  return (totalTime - idleTime)/totalTime;
}