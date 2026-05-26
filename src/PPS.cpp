#include "Pps.hpp"
#include <string>
#include <stdio.h>
#include <sstream>
//Nvidia only atm


PPS::PPS(Logger& logs) : loggerref(logs) 
{
}


void PPS::getfps()
{

std::string message = "NOT IMPLEMENTED";

 loggerref.log("PPS", Logger::Flag::info, "NOT IMPLEMENTED");

}

void  PPS::getframetime()
{
     std::string message = "NOT IMPLEMENTED";
   loggerref.log("PPS", Logger::Flag::info, "The frametime is " +  message + ".");

}

void  PPS::getsummary()
{

    getfps();
    getframetime();
    getcpuload();
    getgpuload();
    getthreadcount();

}

void PPS::getcpuload() 
{
    double cpuload = 0.0;

    std::string gpucommand = "top -bn1| grep %Cpu";

    std::string result = executecommand(gpucommand);

    std::stringstream stringstream(result);

    std::string header ="";
    std::string us = "";
    std::string si = "";
    std::string ny = "";
    std::string idle = "";


    std::getline(stringstream, header, ':');
    std::getline(stringstream, us, 'u'); 
    std::getline(stringstream, ny, ',');
    std::getline(stringstream, idle, ',');  
    

    loggerref.log("PPS", Logger::Flag::info, "CPU usage is at " + us + ".");
}

void PPS::getgpuload()
{
    std::string gpucommand = "nvidia-smi --query-gpu=utilization.gpu --format=csv,noheader";
    std::string result = executecommand(gpucommand);
    loggerref.log("PPS", Logger::Flag::info, "GPU usage is at " + result);
}

void PPS::getmemoryusage()
{
    
    std::string memcommand = "top -bn1 | grep 'MiB Mem'"; // -bn1 for a singular batch version of the command

    std::string result = executecommand(memcommand);

    std::stringstream stringstream(result);
    std::string header = "";
    std::string output = "";
    std::string total = "";
    std::string free = "";
    std::string cache = "";


    std::getline(stringstream, header, ':');
    std::getline(stringstream, total, ',');
    std::getline(stringstream, free, ',');
    std::getline(stringstream, output, ','); 


    loggerref.log("PPS", Logger::Flag::info, "RAM Usage " + output + " out of " + total);
    return;

}

void PPS::getthreadcount()
{
    std::string threadcommand = "grep Threads: /proc/self/status";
    std::string result = executecommand(threadcommand);
    
    std::stringstream stringstream(result);
    std::string header = "";
    std::string output = "";

    std::getline(stringstream, header, ':'); // Gets " Threads"
    std::getline(stringstream, output); // Gets the acutal thread count

    loggerref.log("PPS", Logger::Flag::debug, "Total " + header + " " + output );
    return;
}

std::string PPS::executecommand(std::string command)
{
   

std::string result = "";
FILE* output = popen(command.c_str(), "r"); 

if (!output) // if the command has no output or has returned a error
{
    loggerref.log("PPS", Logger::Flag::error, "Was unable to execute the command " + command);
    return result; // which is ""
}

if (output) // if the command has a output
{
    char line[100]; // buffer of 100 charcters read in at a time.

    while (fgets(line, sizeof(line), output) != nullptr) // while the stream of charcters read from the output buffer is not null 
        {  
            result = result + line; // append to the final string that is to be returned.
        }

    loggerref.log("PPS", Logger::Flag::debug, "Was able to execute the command " + command);
}   
  pclose(output);
  return result;

}

std::string PPS::returnhardwareinfo()
{
    //check if the system is Nvidia or Radeon
    std::string test = "";
    test = executecommand("nvidia-smi --query-gpu=name --format=csv,noheader");
    std::string defaultstring = "Unable to find the correct GPU information.";

    if(test != "")
    {
        std::string result = "";

        std::string gpucommand = "nvidia-smi --query-gpu=name --format=csv,noheader";  //https://nvidia.custhelp.com/app/answers/detail/a_id/3751/%7E/useful-nvidia-smi-queries
        result = executecommand(gpucommand);

        result = result + " | ";

        std::string cpucommand = "lscpu | grep 'Model name'";

        result = result + executecommand(cpucommand);

        loggerref.log("PPS", Logger::Flag::debug, "Nvidia gpu information found.");

        return result;
    }
/*
    test = executecommand(radeon test here) // sudo apt install radeontop

    if(test != "")
    {
        std::string result = "";
        std::string gpucommand = //radeon command here; 

          result = result + " | ";

        std::string cpucommand = "lscpu | grep 'Model name'";

        result = result + executecommand(cpucommand);

        loggerref.log("PPS", Logger::Flag::info, "Raedeon gpu information found.");

        return result;
    }
*/
    loggerref.log("PPS", Logger::Flag::warning, "Unable to find the correct GPU information.");
    return defaultstring;
    
}