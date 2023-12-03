#include <unistd.h>
#include <cctype>
#include <sstream>
#include <string>
#include <vector>
#include <iostream>
#include <fstream> 
#include "process.h"
#include "linux_parser.h"
using std::string;
using std::to_string;
using std::vector;

int Process::Pid() { return this->pid_; }

float Process::CpuUtilization() const {
    std::ifstream filestream("/proc/" + to_string(this->pid_) + "/stat");
    std::string line;
    float utime, stime, cutime, cstime, starttime;
    if (filestream.is_open()) {
        std::getline(filestream, line);
        std::istringstream linestream(line);
        for(int i = 0; i < 13; i++) {
            linestream >> line;
        }
        linestream >> utime >> stime >> cutime >> cstime;
        for(int i = 0; i < 4; i++) {
            linestream >> line;
        }
        linestream >> starttime;
    }
    float total_time = utime + stime;
    float time_elapsed_since_process_started = LinuxParser::UpTime() - (starttime / sysconf(_SC_CLK_TCK));
    float cpu_usage = (total_time / sysconf(_SC_CLK_TCK)) / time_elapsed_since_process_started;
    return cpu_usage;   
}

string Process::Command() { return LinuxParser::Command(this->pid_); }

string Process::Ram() { return LinuxParser::Ram(this->pid_); }

string Process::User() { return LinuxParser::User(this->pid_); }

long int Process::UpTime() { return LinuxParser::UpTime(this->pid_); }

bool Process::operator<(Process const& a) const { 
    return this->CpuUtilization() > a.CpuUtilization();
}