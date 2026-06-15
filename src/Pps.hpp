#pragma once
#include <string>
#include "Logger.hpp"

class PPS 
{

public:

PPS(Logger& logger); 

void  getfps();

void  getframetime();

void  getcpuload();

void  getgpuload();

void  getmemoryusage();

void  getthreadcount();

void  getsummary();

std::string  returnhardwareinfo();

std::string  executecommand(std::string command);

void updatespecs();


private:


Logger& loggerref;

std::string cpuload_;

std::string memoryusage_;

std::string gpuload_;

std::string threadcount_;

bool isNVIDIA_;
};