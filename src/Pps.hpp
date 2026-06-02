#pragma once
#include <string>
#include "Logger.hpp"

/*
struct performanceprofile
{

int fps = 0;

int frametime = 0;

double cpuload 0.0;

double gpuload 0.0;

};

struct supplementalmetrics 
{

    double memoryusage;

    int threadcount;

    std::string hardwareinfo;


};

*/


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



private:

/*
performanceprofile performance;

supplementalmetrics metrics;
*/

Logger& loggerref;

// new private variable to store information for the summary.
std::string cpuload;

std::string memoryusage;

std::string gpuload;

std::string threadcount;
};