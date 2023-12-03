#include <dirent.h>
#include <unistd.h>
#include <sstream>
#include <string>
#include <vector>
#include <unordered_map>
#include "linux_parser.h"
#include <iostream>

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

float LinuxParser::MemoryUtilization() { 
  std::ifstream stream(kProcDirectory + kMeminfoFilename);
  string line;
  string key;
  string value;
  std::unordered_map<string, string> meminfo;
  if (stream.is_open()) {
    while (std::getline(stream, line)) {
      std::replace(line.begin(), line.end(), ':', ' ');
      std::istringstream linestream(line);
      linestream >> key >> value;
      meminfo[key] = value;
    }
  }
  float memoryTotal = stof(meminfo["MemTotal"]);
  float memoryUsed = memoryTotal - stof(meminfo["MemFree"]);
  return (memoryUsed / memoryTotal);
 }

 long LinuxParser::UpTime() {
   std::ifstream stream(kProcDirectory + kUptimeFilename);
   string line;
   string uptime;
   string suspend_time;
   if (stream.is_open()) {
     std::getline(stream, line);
     std::istringstream linestream(line);
     linestream >> uptime >> suspend_time;
   }
   return stol(uptime);
 }

int LinuxParser::TotalProcesses() {
  std::ifstream stream(kProcDirectory + kStatFilename);
  string line;
  string key;
  string value;
  std::unordered_map<string, string> stat;
  if (stream.is_open()) {
    while (std::getline(stream, line)) {
      std::istringstream linestream(line);
      linestream >> key >> value;
      stat[key] = value;
    }
  }
  return stoi(stat["processes"]);
}

int LinuxParser::RunningProcesses() { 
  std::ifstream stream(kProcDirectory + kStatFilename);
  string line;
  string key;
  string value;
  std::unordered_map<string, string> stat;
  if (stream.is_open()) {
    while (std::getline(stream, line)) {
      std::istringstream linestream(line);
      linestream >> key >> value;
      stat[key] = value;
    }
  }
  return stoi(stat["procs_running"]);
 }

string LinuxParser::Command(int pid) {
    std::ifstream stream(kProcDirectory + std::to_string(pid) + kCmdlineFilename);
    string line;
    string command;
    if (stream.is_open()) {
      while(std::getline(stream, line)) {
        command += line;
      }
    }
    return command;
 }

string LinuxParser::Ram(int pid) { 
  std::ifstream stream(kProcDirectory + std::to_string(pid) + kStatusFilename);
  string line;
  string key;
  string value;
  std::unordered_map<string, string> status;
  if (stream.is_open()) {
    while (std::getline(stream, line)) {
      std::replace(line.begin(), line.end(), ':', ' ');
      std::istringstream linestream(line);
      linestream >> key >> value;
      status[key] = value;
    }
  }
  stream.close();
  if(status.find("VmSize") == status.end()) {
    return "0";
  }
  long ram;
  try {
      ram = std::stol(status["VmSize"]) / 1024l;
  } catch (const std::invalid_argument& e) {
      std::cerr << "Invalid argument: " << e.what() << " for VmSize value: " << status["VmSize"] << '\n';
      ram = 0;
  } catch (const std::out_of_range& e) {
      std::cerr << "Out of range: " << e.what() << '\n';
      ram = 0;
  }


   return std::to_string(ram / 1024l);
 }

string LinuxParser::Uid(int pid) { 
  std::ifstream stream(kProcDirectory + std::to_string(pid) + kStatusFilename);
  string line;
  string key;
  string value;
  string uid = "";
  if (stream.is_open()) {
    while (std::getline(stream, line)) {
      std::replace(line.begin(), line.end(), ':', ' ');
      std::istringstream linestream(line);
      linestream >> key >> value;
      if(key == "Uid") {
        uid = value;
        break;
      }
    }
  }
  stream.close();
  return uid;
}

string LinuxParser::User(int pid) {
  string uid = LinuxParser::Uid(pid);
  std::ifstream stream(kPasswordPath);
  string line;
  string userName = "unknown";
  if(stream.is_open()) {
    while(std::getline(stream, line)) {
      std::replace(line.begin(), line.end(), ':', ' ');
      std::istringstream linestream(line);
      string user, x, uid2;
      linestream >> user >> x >> uid2;
      if(uid == uid2) {
        userName = user;
        break;
      }
    }
  }
  stream.close();
  return userName;
 }

long LinuxParser::UpTime(int pid) {
    std::ifstream stream(kProcDirectory + std::to_string(pid) + kStatFilename);
    string line;
    string value;
    if (stream.is_open()) {
      std::getline(stream, line);
      std::istringstream linestream(line);
      for(int i = 0; i < 21; i++) {
        linestream >> value;
      }
      linestream >> value;
    }
    long uptime;
    try {
      uptime = stol(value) / sysconf(_SC_CLK_TCK);
    } catch(const std::exception& e) {
      // std::cerr << e.what() << '\n';
      // std :: cout << "Error: " << e.what() << '\n';
      uptime = 0;
    }
    return LinuxParser::UpTime() - uptime;
 } 
