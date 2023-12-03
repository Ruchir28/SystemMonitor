#include <string>

#include "format.h"

using std::string;

string Format::ElapsedTime(long seconds) { 
    int hours = seconds / 3600;
    int minutes = (seconds % 3600) / 60;
    int secs = seconds % 60;
    string hours_str = hours < 10 ? "0" + std::to_string(hours) : std::to_string(hours);
    string minutes_str = minutes < 10 ? "0" + std::to_string(minutes) : std::to_string(minutes);
    string secs_str = secs < 10 ? "0" + std::to_string(secs) : std::to_string(secs);
    return hours_str + ":" + minutes_str + ":" + secs_str;
}