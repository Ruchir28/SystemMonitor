#include "processor.h"
#include <iostream>
#include "linux_parser.h"
#include "cpu_stat.h"
#include <unordered_map>
#include <vector>


std::vector<float> Processor::Utilization() {
  std::ifstream filestream(LinuxParser::kProcDirectory +
                           LinuxParser::kStatFilename);
  std::string line;
  float user, nice, system, idle, iowait, irq, softirq, steal, guest,
      guest_nice;
  std::string cpu;
  std::unordered_map<std::string, CpuStat> cpuStats;
  static std::unordered_map<std::string, LinuxParser::DiffTracker<CpuStat,float>> cpuStatDiffs;

  if (filestream.is_open()) {
    while(std::getline(filestream, line)) {
        if(line.find("cpu") == std::string::npos) {
            break;
        }
        std::istringstream linestream(line);
        linestream >> cpu >> user >> nice >> system >> idle >> iowait >> irq >> softirq >> steal >> guest >> guest_nice;
        CpuStat cpuStat(cpu, user, nice, system, idle, iowait, irq, softirq, steal, guest, guest_nice);
        if(cpuStatDiffs.find(cpu) == cpuStatDiffs.end()) {
            cpuStatDiffs[cpu] = LinuxParser::DiffTracker<CpuStat,float>(cpuStat);
        }
        // LinuxParser::DiffTracker<CpuStat,float> &totalD = cpuStatDiffs[cpu];
        cpuStats[cpu] = cpuStat;
        cpuStatDiffs[cpu].update(cpuStat);
    }
  }
  filestream.close();
  std::vector<float> cpu_util_vec;
  for(auto &cpuStatDiff : cpuStatDiffs) {
    cpu_util_vec.push_back(cpuStatDiff.second.Difference());
  }

  return cpu_util_vec;
}

