#include <unistd.h>
#include <cctype>
#include <sstream>
#include <string>
#include <vector>
#include "linux_parser.h"
#include "process.h"

using std::string;
using std::to_string;
using std::vector;

Process::Process(int pid) {
    pid_ = pid;
  	
}

// DONE: Return this process's ID
int Process::Pid() { return pid_; }

// DONE: Return this process's CPU utilization
float Process::CpuUtilization() const{ 
    float utilization = 0.0;
	float timeDiff = (float)LinuxParser::UpTime(pid_);
	if (timeDiff > 0)
      utilization = ((float)LinuxParser::ActiveJiffies(pid_)/sysconf(_SC_CLK_TCK)) /timeDiff ;
    
	return utilization;
}


// DONE: Return the command that generated this process
string Process::Command() { return LinuxParser::Command(pid_);; }

// DONE: Return this process's memory utilization
string Process::Ram() { return LinuxParser::Ram(pid_); }

// DONE: Return the user (name) that generated this process
string Process::User() { return LinuxParser::User(pid_); }

// DONE: Return the age of this process (in seconds)
long int Process::UpTime() { return LinuxParser::UpTime(pid_); }

// DONE: Overload the "less than" comparison operator for Process objects
bool Process::operator<(const Process& a) const {
    return CpuUtilization() < a.CpuUtilization(); 

}