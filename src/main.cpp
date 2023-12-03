#include "ncurses_display.h"
#include "system.h"
#include <iostream>
#include "linux_parser.h"
#include <thread>
#include <format.h>
int main() {
  System system;
  // while(1){
  //   Process process(568803);
  //   std::cout << "CPU Utilization: " << process.CpuUtilization() << std::endl;
  //   std::cout << "Command: " << process.Command() << std::endl;
  //   std::cout << "Time:" << process.UpTime() << std::endl;
  //   std::cout << "Formatted Time:" << Format::ElapsedTime(process.UpTime()) << std::endl;
  //   std::this_thread::sleep_for(std::chrono::milliseconds(1000));
  // }
  // std::cout << LinuxParser::Ram(2) << std::endl;
  NCursesDisplay::Display(system);
  

  
}