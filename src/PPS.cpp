#include "Pps.hpp"
#include <string>
#include <stdio.h>
#include <sstream>

#ifdef _WIN32
#include <windows.h> // to get proccess ID for thread count
#endif


PPS::PPS(Logger& logs) : loggerref(logs) 
{
}


void PPS::getfps()
{

    std::string message = "NOT IMPLEMENTED";

<<<<<<< HEAD
    loggerref.log("PPS", Logger::Flag::info, "NOT IMPLEMENTED");
=======
 loggerref.log("PPS", LogFlag::Info, "NOT IMPLEMENTED");
>>>>>>> 106f6c5dab704babdbcbb0105e65244f231a4d53

}

void  PPS::getframetime()
{
<<<<<<< HEAD
    std::string message = "NOT IMPLEMENTED";
    loggerref.log("PPS", Logger::Flag::info, "The frametime is " +  message + ".");
=======
     std::string message = "NOT IMPLEMENTED";
   loggerref.log("PPS", LogFlag::Info, "The frametime is " +  message + ".");
>>>>>>> 106f6c5dab704babdbcbb0105e65244f231a4d53

}

void  PPS::getsummary()
{

    std::string message = "";   
    //fps function
    //frame time function
    getthreadcount();
    getmemoryusage();
    getcpuload();
    getgpuload();
    message = "CPU load " + cpuload + "%" + " | " + memoryusage  + " | " + gpuload + " | " + threadcount + " |";
    loggerref.log("PPS", Logger::Flag::info,  message );

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

<<<<<<< HEAD
    us.erase(0,2); // removing spaces

    #endif

    loggerref.log("PPS", Logger::Flag::debug, "CPU usage is at " + us + ".");

    cpuload = us; // storing into private variable to be called later in the summary

=======
    loggerref.log("PPS", LogFlag::Info, "CPU usage is at " + us + ".");
>>>>>>> 106f6c5dab704babdbcbb0105e65244f231a4d53
}

void PPS::getgpuload()
{
    std::string gpucommand = "nvidia-smi --query-gpu=utilization.gpu --format=csv,noheader";
    std::string result = executecommand(gpucommand);
<<<<<<< HEAD
    loggerref.log("PPS", Logger::Flag::debug, "GPU usage is at " + result);
    gpuload = "GPU usage is at " + result;
    gpuload.pop_back(); // remove 

=======
    loggerref.log("PPS", LogFlag::Info, "GPU usage is at " + result);
>>>>>>> 106f6c5dab704babdbcbb0105e65244f231a4d53
}

void PPS::getmemoryusage()
{
    //TODO currently for windows it only find avaiable memory
    #ifdef _WIN32
    std::string availablememory;
    std::string result = executecommand("systeminfo |find 'Available Physical Memory'");
    std::stringstream stringstream(result);
    std::string header = "";
    std::string output = "";
    std::getline(stringstream, header, ':');
    std::getline(stringstream, output);




    availablememory = output;



    result = executecommand("systeminfo |find 'Total Physical Memory'");
    std::stringstream stringstream2(result);
    header = "";
    output = "";
    std::getline(stringstream2, header, ':');
    std::getline(stringstream2, output);







    loggerref.log("PPS", Logger::Flag::info, "Available Memory " +   availablememory + " out of " + output);

    memoryusage = "Available Memory " + availablememory + " out of " + output;


   


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


<<<<<<< HEAD
    loggerref.log("PPS", Logger::Flag::info, "RAM Usage " + output + "MB out of " + total + "MB.");

    output.erase(0,4);

    total.erase(0,1);

    memoryusage = "Memory Usage " + output + total;

=======
    loggerref.log("PPS", LogFlag::Info, "RAM Usage " + output + " out of " + total);
>>>>>>> 106f6c5dab704babdbcbb0105e65244f231a4d53
    return;

    #endif

}

void PPS::getthreadcount()
{

   #ifdef _WIN32

    std::string threadcommand = "powershell -Command '(Get-Process -Id " + std::to_string(GetCurrentProcessId()) + ").Threads.Count'";

    std::string result = executecommand(threadcommand);

    std::stringstream stringstream(result);

    std::string header = "";
    std::string output = "";


    std::getline(stringstream, header); 
    
    std::getline(stringstream, output); 


    #else
    // Need to do make the thread count exlclusive to whatever process ID the program is running in.

    std::string threadcommand = "grep Threads: /proc/self/status";
    std::string result = executecommand(threadcommand);
    
    std::stringstream stringstream(result);
    std::string header = "";
    std::string output = "";

    std::getline(stringstream, header, ':'); // Gets " Threads"
    std::getline(stringstream, output); // Gets the acutal thread count
  
    output.erase(0,1);

  #endif



   loggerref.log("PPS", Logger::Flag::debug, "Total " + header + " " + output );

   threadcount = "Total " + header + " " + output;

  return;


<<<<<<< HEAD
=======
    loggerref.log("PPS", LogFlag::Debug, "Total " + header + " " + output );
    return;
>>>>>>> 106f6c5dab704babdbcbb0105e65244f231a4d53
}

std::string PPS::executecommand(std::string command)
{
   

std::string result = "";
#ifdef _WIN32

FILE* output = _popen(command.c_str(), "r"); 

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
  _pclose(output);

  return result;



#else

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
  



#endif

    return result;

}

std::string PPS::returnhardwareinfo()
{

    std::string test;
    test = executecommand("nvidia-smi --query-gpu=name --format=csv,noheader");
    std::string defaultstring = "Unable to find the correct GPU information.";

    if(test != "") // TODO currently this simply checks if the test command doesnt return anything - there needs to be a more airtight way to check this.
    {

        #ifdef _WIN32
   

        std::string result = "";
        std::string result2 = "";

        std::string gpucommand = "wmic path win32_videocontroller get name"; //https://manpages.debian.org/experimental/rocm-smi/rocm-smi.1.en.html
        std::string cpucommand = "wmic cpu get name"; // mabye switch this later wmic is outdates on some devices

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



        loggerref.log("PPS", Logger::Flag::info, output);

        return output;





       #else

        std::string result = "";

        std::string gpucommand = "nvidia-smi --query-gpu=name --format=csv,noheader";  //https://nvidia.custhelp.com/app/answers/detail/a_id/3751/%7E/useful-nvidia-smi-queries
        result = executecommand(gpucommand);

        result = result + " | ";

        std::string cpucommand = "lscpu | grep 'Model name'";

        result = result + executecommand(cpucommand);

        loggerref.log("PPS", LogFlag::Debug, "Nvidia gpu information found.");

        return result;

     #endif

    }

  
    if(test == "")

    {
          #ifdef _WIN32

         std::string result = "";
         std::string result2 = "";

        std::string gpucommand = "wmic path win32_videocontroller get name"; //https://manpages.debian.org/experimental/rocm-smi/rocm-smi.1.en.html
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



        loggerref.log("PPS", Logger::Flag::info, output);

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

        loggerref.log("PPS", Logger::Flag::info, output);

        return output;

        #endif
    }
*/
    loggerref.log("PPS", LogFlag::Warning, "Unable to find the correct GPU information.");
    return defaultstring;
    
}