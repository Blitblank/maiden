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


                    finalmessage = finalmessage + "[" +  "Not Implemented" + "] "; 

                }

        
            finalmessage = finalmessage + "[" + component + "] "; // component is the section of the program (For example Mesh or Engine) that is calling the logger
                    
                std::string level = "";

            // Doing this manually for now depending on if we add more flags I will change this later.
            switch (flag)
            {
            case Flag::debug:
                level = "debug";
                break;

            case Flag::info:
                level = "info";
                break;

            case Flag::warning:
                level = "warning";
            break;

            case Flag::error:
                level = "error";
            break;
            
            default:
                level = "Unknown";
            break;
            }

            finalmessage = finalmessage + "[" + level + "] "; 

            finalmessage = finalmessage + message; 

            if (additionaldetails)
                {
                    finalmessage = finalmessage + " " + "[Line " + std::to_string(Source.line()) + "]"  + "["  +  Source.function_name() + "]";
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