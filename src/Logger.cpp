#include <chrono> // Tracking the time when the log function is called
#include <string>
#include <source_location> 
#include "Logger.hpp"
#include <iostream>
#include <fstream>


    Logger::Logger(ConfigService* config, const std::string& loggerId)
        {
            // TODO: parse strings of config to the enums
            minimumFlag = Flag::debug;

            auto loggerConfigurationRaw = config->getLoggerConfig(loggerId);
            if(!loggerConfigurationRaw) {
                std::cout << "Failed to get logger configuration fom config service" << std::endl;
                return;
            }
            LoggerConfig loggerConfiguration = loggerConfigurationRaw.value();

            StandardOutput = loggerConfiguration.coutEnabled;
            FileOutput = loggerConfiguration.fileEnabled;
            additionaldetails = loggerConfiguration.showSourceTrace;
            time = loggerConfiguration.showTime;

            if (FileOutput)
            {
                outfile.open(loggerConfiguration.filePath);
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