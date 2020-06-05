#include <dirent.h>
#include <unistd.h>
#include <string>
#include <vector>

#include "linux_parser.h"

using std::stof;
using std::string;
using std::to_string;
using std::vector;

// DONE: An example of how to read data from the filesystem
string LinuxParser::OperatingSystem() {
  string line;
  string key;
  string value;
  std::ifstream filestream(kOSPath);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::replace(line.begin(), line.end(), ' ', '_');
      std::replace(line.begin(), line.end(), '=', ' ');
      std::replace(line.begin(), line.end(), '"', ' ');
      std::istringstream linestream(line);
      while (linestream >> key >> value) {
        if (key == "PRETTY_NAME") {
          std::replace(value.begin(), value.end(), '_', ' ');
          return value;
        }
      }
    }
  }
  return value;
}

// DONE: An example of how to read data from the filesystem
string LinuxParser::Kernel() {
  string os, kernel, version;
  string line;
  std::ifstream stream(kProcDirectory + kVersionFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    linestream >> os >> version >> kernel;
  }
  return kernel;
}

// BONUS: Update this to use std::filesystem
vector<int> LinuxParser::Pids() {
  vector<int> pids;
  DIR* directory = opendir(kProcDirectory.c_str());
  struct dirent* file;
  while ((file = readdir(directory)) != nullptr) {
    // Is this a directory?
    if (file->d_type == DT_DIR) {
      // Is every character of the name a digit?
      string filename(file->d_name);
      if (std::all_of(filename.begin(), filename.end(), isdigit)) {
        int pid = stoi(filename);
        pids.push_back(pid);
      }
    }
  }
  closedir(directory);
  return pids;
}

// DONE: Read and return the system memory utilization
float LinuxParser::MemoryUtilization() {  
  string line;
  string key;
  string value;
  float total = 0.0;
  float free = 0.0;  
  
  std::ifstream filestream(kProcDirectory + kMeminfoFilename);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {      
      std::remove(line.begin(), line.end(), ' ');
      std::replace(line.begin(), line.end(), ':', ' ');
      std::istringstream linestream(line);
      while (linestream >> key >> value) {    
        if ("MemTotal" == key ) {
          total = stof(value);
        }      
        else if ("MemFree" == key) {
          free = stof(value);
          break;
        }
      }
    }
  } 
  return ((total - free) / total);
}

// DONE: Read and return the system uptime
long LinuxParser::UpTime() { 
  string upTime;
  string line;
  std::ifstream stream(kProcDirectory + kUptimeFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    linestream >> upTime;
	return stol(upTime);
  }
  else return 0; 

}

// TODO: Read and return the number of jiffies for the system
long LinuxParser::Jiffies() { 
  string line;
  string cpu;  
  std::ifstream filestream(kProcDirectory+kStatFilename);
  
  if (filestream.is_open()) {
	string cpu, user, nice, system, idle, iowait, irq, softirq, steal, guest, guestNice;
    std::getline(filestream, line);
    std::istringstream linestream(line);
    linestream >> cpu >> user >> nice >> system >> idle >> iowait >> irq >> softirq >> steal >> guest >> guestNice;
	
	return (std::stol(user)+std::stol(nice)+std::stol(system)+std::stol(idle)+
          std::stol(iowait)+std::stol(irq)+std::stol(softirq)+ std::stol(steal)+
          std::stol(guest)+std::stol(guestNice));
  }
  else return 0; 


}

// TODO: Read and return the number of active jiffies for a PID
long LinuxParser::ActiveJiffies(int pid) {
  string line; 
  string key;
  std::ifstream filestream(kProcDirectory+to_string(pid)+kStatFilename);
  long value = 0;
  if (filestream.is_open()) {
    std::getline(filestream, line);
    std::istringstream linestream(line);
	string uTime, stime,cuTime,csTime;
    for (int i=1; i<14; i++)
      linestream >> key;
    linestream >> uTime >> stime>> cuTime >>csTime;
	
	value = (stol(uTime)+stol(stime)+stol(cuTime)+stol(csTime));
  }
  return value;
}
// DONE: Read and return the number of active jiffies for the system
long LinuxParser::ActiveJiffies() { 
  string line;
  string cpu;  
  std::ifstream filestream(kProcDirectory+kStatFilename);
  long value = 0;
  if (filestream.is_open()) {
	string cpu, user, nice, system, idle, iowait, irq, softirq, steal, guest, guestNice;
    std::getline(filestream, line);
    std::istringstream linestream(line);
    linestream >> cpu >> user >> nice >> system >> idle >> iowait >> irq >> softirq >> steal >> guest >> guestNice;
	
	value = (stol(user)+stol(nice)+stol(system)+stol(irq)+stol(softirq)+stol(steal)+stol(guest)+ stol(guestNice));
  }
  return value; 
}

// TODO: Read and return the number of idle jiffies for the system
long LinuxParser::IdleJiffies() { 
  string line;
  string cpu;  
  std::ifstream filestream(kProcDirectory+kStatFilename);
  long value = 0;
  if (filestream.is_open()) {
	string cpu, user, nice, system, idle, iowait, irq, softirq, steal, guest, guestNice;
    std::getline(filestream, line);
    std::istringstream linestream(line);
    linestream >> cpu >> user >> nice >> system >> idle >> iowait >> irq >> softirq >> steal >> guest >> guestNice;
	
	value = (stol(idle)+stol(iowait));
  }
  return value;  
}

// DONE: Read and return CPU utilization
vector<string> LinuxParser::CpuUtilization() { 
  string line;
  std::ifstream fileStream(kProcDirectory + kStatFilename);
  if (fileStream.is_open()) {
    std::getline(fileStream, line);
    std::istringstream lineStream(line);
    std::vector<string> infos{std::istream_iterator<string>{lineStream}, 
    std::istream_iterator<string>{}};
    infos.erase(infos.begin());
    return infos; 
  }
  return {};
}
// DONE: Read and return the total number of processes
int LinuxParser::TotalProcesses() { 
  string line;
  string key;
  string value;
  std::ifstream filestream(kProcDirectory + kStatFilename);
  
  if (filestream.is_open()) {
    while(std::getline(filestream, line)){
      std::istringstream linestream(line);
      linestream >> key >> value;
      if (key == "processes") {
        return stoi(value);
      }
    }
  }
  return 0;
 }
// DONE: Read and return the number of running processes
int LinuxParser::RunningProcesses() {
  string line;
  string key;
  string value;
  std::ifstream filestream(kProcDirectory+kStatFilename);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::istringstream linestream(line);
      linestream >> key >> value;
      if (key == "procs_running") 
          return std::stoi(value); 
    }
  }
  return 0;
}

// DONE: Read and return the command associated with a process

string LinuxParser::Command(int pid) {
  string line;
  std::ifstream filestream(kProcDirectory + to_string(pid) + kCmdlineFilename);
  if (filestream.is_open()) {
    std::getline(filestream, line);
    return line;
  }
  return "Na";
}

// DONE: Read and return the memory used by a process
string LinuxParser::Ram(int pid) {
  string line;
  string key;
  string value;
  std::ifstream filestream(kProcDirectory + to_string(pid) + kStatusFilename);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::istringstream linestream(line);
      while (linestream >> key >> value) {
        if (key == "VmSize:") {
          return to_string(stoi(value) / 1024);
        }
      }
    }
  }
  return "Na";
}
// DONE: Read and return the user ID associated with a process
string LinuxParser::Uid(int pid) {
  string line;
  string key;
  string value;
  std::ifstream filestream(kProcDirectory + to_string(pid) + kStatusFilename);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::istringstream linestream(line);
      linestream >> key >> value;
      if (key == "Uid:") {
        return value;
      }      
    }
  }
  return "Na";
}
// DONE: Read and return the user associated with a process
string LinuxParser::User(const int pid) { 
  string line;
  string key;
  string value;
  std::ifstream fileStream(LinuxParser::kPasswordPath);
  string name, x, id;
  if (fileStream.is_open()){
    while(std::getline(fileStream, line)) {
      std::replace(line.begin(), line.end(), ':', ' ');
      std::istringstream lineStream(line);
      lineStream >> name >> x >> id; 
      if (id == LinuxParser::Uid(pid)) {
        return name;
      }
    }
  }
  return "Na";
}
// DONE: Read and return the uptime of a process
long LinuxParser::UpTime(const int pid) { 
  std::ifstream fileStream(kProcDirectory + to_string(pid) + kStatFilename);
  string line;
  if (fileStream.is_open()) {
    std::getline(fileStream, line);
    std::istringstream lineStream(line);
    std::vector<std::string> data{std::istream_iterator<string>{lineStream}, std::istream_iterator<string>{}};
    return LinuxParser::UpTime() - std::stol(data[21])/sysconf(_SC_CLK_TCK);
  }
  return 0;
}