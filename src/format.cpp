#include <string>

#include "format.h"
#include <iostream>
#include <iomanip>
using std::string;


// INPUT: Long int measuring seconds
// OUTPUT: HH:MM:SS

std::string Format::ElapsedTime(long seconds) { 
       
    const long ss = seconds % 60;
    const long mm = (seconds / 60) % 60;
    const long hh   = seconds / 3600; 

    std::stringstream time;

    time << std::setfill('0') << std::setw(2) << hh << ":";
    time << std::setfill('0') << std::setw(2) << mm << ":";  
    time << std::setfill('0') << std::setw(2) << ss; 

    return time.str();
}