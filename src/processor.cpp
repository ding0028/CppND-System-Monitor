#include "processor.h"
#include "linux_parser.h"

Processor::Processor() {
  idleTime = LinuxParser::IdleJiffies();  
  totalTime = LinuxParser::Jiffies();	
}

float Processor::Utilization() {
  
  float curIdleTime = LinuxParser::IdleJiffies()- idleTime;  
  float curTotalTime = LinuxParser::Jiffies()- totalTime;
  if (curTotalTime <= 0 || curIdleTime <= 0)
    return 0;
  return (curTotalTime - curIdleTime)/curTotalTime;
}