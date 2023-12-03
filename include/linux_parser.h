#ifndef SYSTEM_PARSER_H
#define SYSTEM_PARSER_H

#include <fstream>
#include <regex>
#include <string>

namespace LinuxParser {
// Paths
const std::string kProcDirectory{"/proc/"};
const std::string kCmdlineFilename{"/cmdline"};
const std::string kCpuinfoFilename{"/cpuinfo"};
const std::string kStatusFilename{"/status"};
const std::string kStatFilename{"/stat"};
const std::string kUptimeFilename{"/uptime"};
const std::string kMeminfoFilename{"/meminfo"};
const std::string kVersionFilename{"/version"};
const std::string kOSPath{"/etc/os-release"};
const std::string kPasswordPath{"/etc/passwd"};

// System
float MemoryUtilization();
long UpTime();
std::vector<int> Pids();
int TotalProcesses();
int RunningProcesses();
std::string OperatingSystem();
std::string Kernel();

// CPU
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
std::vector<std::string> CpuUtilization();

// Processes
std::string Command(int pid);
std::string Ram(int pid);
std::string Uid(int pid);
std::string User(int pid);
long int UpTime(int pid);

// Util To Track Difference
template <typename T, typename DiffType>
class DiffTracker {
 public:
  DiffTracker() {}
  DiffTracker(T value) : value(value), prev_value(value) {}
  DiffTracker(T value, T prev_value) : value(value), prev_value(prev_value) {}
  void update(T updatedValue) {
    this->prev_value = this->value;
    this->value = updatedValue;
  }
  DiffType Difference() const {
    DiffType diff = value - prev_value;
    return diff;
  }
  T Value() const { return value; }
  T PrevValue() const { return prev_value; }

 private:
  T value;
  T prev_value;
};

};  // namespace LinuxParser

#endif