#include "Pps.hpp"
#include <string>
#include <stdio.h>
#include <sstream>
#include <iostream>
#ifdef _WIN32
#include <windows.h> // to get proccess ID for thread count
#endif


PPS::PPS(Logger& logs) : loggerref(logs)
{
    std::string test;


    test = executecommand("nvidia-smi --query-gpu=name --format=csv,noheader");


    isNVIDIA_ = true;


    if (test.empty())
    {
        isNVIDIA_ = false;
    }
   
    if(isNVIDIA_)
    {
    loggerref.log("PPS", LogFlag::Debug, "Performance profilier initialized");
    }


    else
    {
    loggerref.log("PPS", LogFlag::Debug, "Performance profilier initialized no NVIDIA GPU found");
    }
}


void PPS::getfps()
{

    std::string message = "NOT IMPLEMENTED";

 loggerref.log("PPS", LogFlag::Info, "NOT IMPLEMENTED");

}

void  PPS::getframetime()
{
     std::string message = "NOT IMPLEMENTED";
   loggerref.log("PPS", LogFlag::Info, "The frametime is " +  message + ".");

}

void  PPS::getsummary()
{
    updatespecs();
    std::string message = "";  
    message =  "CPU usage " + cpuload_  + "%" + " | " + memoryusage_  + " | GPU usage is at " + gpuload_ + " | " + threadcount_ + " |";
    loggerref.log("PPS", LogFlag::Info,  message );
}
void PPS::updatespecs()
{
    //fps function
    //frame time function
    getthreadcount();
    getmemoryusage();
    getcpuload();
    getgpuload();
}

void PPS::getcpuload()
{

    #ifdef _WIN32

    std::string cpucommand = "wmic cpu get loadpercentage"; //https://stackoverflow.com/questions/9097067/get-cpu-usage-from-vcb windows-command-prompt

    std::string result = executecommand(cpucommand);

    std::stringstream stringstream(result);

    std::string header ="";

    std::string us = "";

    std::getline(stringstream, header);

    std::getline(stringstream, us);

   us.pop_back();
   while(us.back() == ' ') // Removing blankspace
    {
        us.pop_back();
    }

    #else

    std::string cpucommand = "top -bn1| grep %Cpu";

    std::string result = executecommand(cpucommand);

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

    us.erase(0,2); // removing spaces

    #endif

    loggerref.log("PPS", LogFlag::Info, "CPU usage is at " + us + ".");

    cpuload_ = us;

}

void PPS::getgpuload()
{
    if (isNVIDIA_)
    {
    std::string gpucommand = "nvidia-smi --query-gpu=utilization.gpu --format=csv,noheader";
    std::string result = executecommand(gpucommand);
   loggerref.log("PPS", LogFlag::Info, "GPU usage is at " + result);
    gpuload_ =  result;
    if (!gpuload_.empty())
    {
    gpuload_.pop_back(); // remove
    }
    }
    else
    {
        #ifdef _WIN32
        std::string gpucommand = "TODO ADD HERE";
        std::string result = executecommand(gpucommand);
        loggerref.log("PPS", LogFlag::Info, "GPU usage is at " + result);
        gpuload_ =  result;
        gpuload_.pop_back(); // remove
        #else
        std::string gpucommand = "TODO ADD HERE";
        std::string result = executecommand(gpucommand);
        loggerref.log("PPS", LogFlag::Info, "GPU usage is at " + result);
        gpuload = result;
        gpuload.pop_back(); // remove
        #endif
    }

}

void PPS::getmemoryusage()
{
    #ifdef _WIN32
    std::string availablememory;
    std::string result = executecommand("systeminfo |find \"Available Physical Memory\"");
    std::stringstream stringstream(result);
    std::string header = "";
    std::string output = "";
    std::getline(stringstream, header, ':');
    std::getline(stringstream, output);




    availablememory = output;



    result = executecommand("systeminfo |find \"Total Physical Memory\"");
    std::stringstream stringstream2(result);
    header = "";
    output = "";
    std::getline(stringstream2, header, ':');
    std::getline(stringstream2, output);




    if (!availablememory.empty())
    {
        while (availablememory.front() == ' ')
        {
            availablememory.erase(0,1); // Erase only the first character
        }
    }
    if (!output.empty())
    {
        while (output.front() == ' ')
    {
            output.erase(0,1);
    }
    }
    loggerref.log("PPS", LogFlag::Info, "Available Memory " + availablememory + " out of " + output);


    memoryusage_ = "Available Memory " + availablememory + " out of " + output;
   




    #else

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


      loggerref.log("PPS", LogFlag::Info, "RAM Usage " + output + " out of " + total);

    output.erase(0,4);

    total.erase(0,1);

    memoryusage = "Memory Usage " + output + total;

    return;

    #endif

}

void PPS::getthreadcount()
{

   #ifdef _WIN32

    std::string threadcommand = "powershell -Command \"(Get-Process -Id " + std::to_string(GetCurrentProcessId()) + ").Threads.Count\"";

    std::string result = executecommand(threadcommand);

    std::stringstream stringstream(result);

    std::string header = "";
    std::string output = "";


    std::getline(stringstream, header);
   
    std::getline(stringstream, output);


    #else

    std::string threadcommand = "grep Threads: /proc/self/status";
    std::string result = executecommand(threadcommand);
   
    std::stringstream stringstream(result);
    std::string header = "";
    std::string output = "";

    std::getline(stringstream, header, ':'); // Gets " Threads"
    std::getline(stringstream, output); // Gets the acutal thread count
 
    while(output.front == ' ')
    {
     output.erase(0,1);
    }
   
  #endif



loggerref.log("PPS", LogFlag::Debug, "Thread count " + header + " " + output );

   threadcount_ = "Thread count " + header + " " + output;

  return;



}

std::string PPS::executecommand(std::string command)
{


std::string result = "";
#ifdef _WIN32

FILE* output = _popen(command.c_str(), "r");

if (!output) // if the command has no output
{
    loggerref.log("PPS", LogFlag::Error, "Was unable to execute the command " + command);
    return result; // which is ""
}

if (output) // if the command has a output
{
    char line[100]; // buffer of 100 charcters read in at a time.

    while (fgets(line, sizeof(line), output) != nullptr) // while the stream of charcters read from the output buffer is not null
        {  
            result = result + line; // append to the final string that is to be returned.
        }

    loggerref.log("PPS", LogFlag::Debug, "Was able to execute the command " + command);
    std::cout << "\n";
}  
  _pclose(output);




#else

FILE* output = popen(command.c_str(), "r");
if (!output) // if the command has no output or has returned a error
{
    loggerref.log("PPS", LogFlag::Error, "Was unable to execute the command " + command);
    return result; // which is ""
}

if (output) // if the command has a output
{
    char line[100]; // buffer of 100 charcters read in at a time.

    while (fgets(line, sizeof(line), output) != nullptr) // while the stream of charcters read from the output buffer is not null
        {  
            result = result + line; // append to the final string that is to be returned.
        }

    loggerref.log("PPS", LogFlag::Debug, "Was able to execute the command " + command);
}  
    pclose(output);




#endif
//Removing the newline characters which windows adds to the end of commands
    if(!result.empty())
    {
        if(result.back() == '\n')
        {
            result.pop_back();
        }
    }

    return result;

}

std::string PPS::returnhardwareinfo()
{

    std::string test;
    test = executecommand("nvidia-smi --query-gpu=name --format=csv,noheader");
    std::string defaultstring = "Unable to find the correct GPU information.";

    if(isNVIDIA_)
    {

        #ifdef _WIN32


        std::string result = "";
        std::string result2 = "";

        std::string gpucommand = "wmic path win32_videocontroller get name";
        std::string cpucommand = "wmic cpu get name";

        result = executecommand(cpucommand);

        std::stringstream stringstream(result);

        std::string header = "";
        std::string output = "";


        std::getline(stringstream, header);
        std::getline(stringstream, output);


        output = output + " | ";



        result2 = executecommand(gpucommand);

        std::stringstream stringstream2(result2);


        std::string header2 = "";
        std::string output2 = "";

        std::getline(stringstream2, header2);
        std::getline(stringstream2, output2);


        output = output + output2;



        loggerref.log("PPS", LogFlag::Info, output);

        return output;





       #else

        std::string result = "";

        std::string gpucommand = "nvidia-smi --query-gpu=name --format=csv,noheader";
        result = executecommand(gpucommand);

        result = result + " | ";

        std::string cpucommand = "lscpu | grep 'Model name'";

        result = result + executecommand(cpucommand);

        loggerref.log("PPS", LogFlag::Debug, "Nvidia gpu information found.");

        return result;

     #endif

    }


 else

    {
          #ifdef _WIN32

         std::string result = "";
         std::string result2 = "";

        std::string gpucommand = "wmic path win32_videocontroller get name";
        std::string cpucommand = "wmic cpu get name";

        result = executecommand(cpucommand);

        std::stringstream stringstream(result);

        std::string header = "";
        std::string output = "";

        std::getline(stringstream, header);
        std::getline(stringstream, output);


        output = output + " | ";



        result2 = executecommand(gpucommand);

        std::stringstream stringstream2(result2);


        std::string header2 = "";
        std::string output2 = "";

        std::getline(stringstream2, header2);
        std::getline(stringstream2, output2);


        output = output + output2;



        loggerref.log("PPS", LogFlag::Info, output);

        return output;



       #else

        std::string result = "";

        std::string gpucommand = "library to fetch device information here"; //https://manpages.debian.org/experimental/rocm-smi/rocm-smi.1.en.html
        std::string cpucommand = "lscpu | grep 'Model name'";

        result = executecommand(cpucommand);

        std::stringstream stringstream(result);

        std::string header = "";
        std::string output = "";

        std::getline(stringstream, header, ':');
        std::getline(stringstream, output);


        output = output + " | ";

        output = output + executecommand(gpucommand);

        loggerref.log("PPS", LogFlag::Info, output);

        return output;

        #endif
    }

    loggerref.log("PPS", LogFlag::Warning, "Unable to find the correct GPU information.");
    return defaultstring;
   
}