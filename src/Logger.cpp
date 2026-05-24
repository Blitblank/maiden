#include <chrono> // Tracking the time when the log function is called
#include <string>
#include <source_location> 
#include "Logger.hpp"
#include <iostream>
#include <fstream>


    Logger::Logger(Flag MinimumFlag, bool SOlog, bool FileLog, std::string Filepath, bool Additionaldetails, bool Time)

        {
            minimumFlag = MinimumFlag;
            StandardOutput = SOlog; // Standard output
            FileOutput = FileLog; 
            additionaldetails = Additionaldetails;
            time = Time;

            if (FileOutput)
            {
                outfile.open(Filepath);
            }

            std::cout << "Logger initialized " << std::endl;
        }
    
    Logger::~Logger()
        {
            if(outfile)
            {
                outfile.close();
            }
        }

    void Logger::log(std::string component, Flag flag, std::string message, std::source_location Source)

        {
            if (!(flag >= minimumFlag)) 
                {
                    return;
                }

            std::string finalmessage = "";

            if (time)
                {
                    // https://en.cppreference.com/cpp/chrono/system_clock/now

                    const auto now = std::chrono::system_clock::now();
                    const std::time_t t_c = std::chrono::system_clock::to_time_t(now);

                    std::string finaltime = std::ctime(&t_c);
                    finaltime.pop_back();

                    finalmessage = finalmessage + "[" +  finaltime + "] "; 

                }

        
            finalmessage = finalmessage + "[" + component + "] "; // component is the section of the program (For example Mesh or Engine) that is calling the logger
                    
                std::string level = "";

            // Doing this manually for now depending on if we add more flags I will change this later.
            switch (flag)
            {
            case Flag::debug:
                level = "DEBUG";
                break;

            case Flag::info:
                level = "INFO";
                break;

            case Flag::warning:
                level = "WARNING";
            break;

            case Flag::error:
                level = "ERROR";
            break;
            
            default:
                level = "UNKNOWN";
            break;
            }

            finalmessage = finalmessage + "[" + level + "] "; 

            finalmessage = finalmessage + message + " "; 

            if (additionaldetails)
                {
                    finalmessage = finalmessage  + "[Function: " + Source.function_name() + "]"  +" " + "[Line: "  +  std::to_string(Source.line()) + "]" + " " + "[File: "  +  Source.file_name() + "]";
                }
   
            if (StandardOutput)
                {
                    std::cout << std::endl;
                    std::cout << finalmessage << std::endl;
                }

            if (FileOutput)
                {
                    outfile << std::endl;
                    outfile << finalmessage << std::endl;
                }
            return;
        }