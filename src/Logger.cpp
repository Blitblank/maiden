#include <chrono> // Tracking the time when the log function is called
#include <string>
#include <source_location> 
#include "Logger.hpp"
#include <iostream>
#include <iomanip>
#include <fstream>


    Logger::Logger(ConfigService* config, const std::string& loggerId)
        {

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
            id = loggerConfiguration.id;
            
            for(std::string& flag : loggerConfiguration.flagsEnabled) {
                bool found = false;
                for(const char* validFlag : LogFlagStrings) {
                    if(flag == std::string(validFlag)) {
                        found = true;
                        for(int i = 0; i < LogFlag::Count; i++) {
                            if(LogFlagStrings[i] == flag) {
                                activeFlags.emplace_back(static_cast<LogFlag>(i));
                                break;
                            }
                        }
                    }
                }
                if(!found) {
                    std::cout << "Log flag '" << flag << "' in configuration file is not a valid log flag" << std::endl;
                }
            }


              const auto now = std::chrono::system_clock::now();
                    const std::time_t t_c = std::chrono::system_clock::to_time_t(now);

                    std::string finaltime = std::ctime(&t_c);

                    finaltime.pop_back(); // removing the newline

                std::replace(finaltime.begin(),finaltime.end(), ':' , '-'); // filenames cant have dashes

            if (FileOutput)
            {
                outfile.open(loggerConfiguration.filePath);
            }

            log("Logger", LogFlag::Info, "Logger initialized.");
        }
    
    Logger::~Logger()
        {
            if(outfile)
            {
                outfile.close();
            }
        }

    void Logger::log(std::string component, LogFlag flag, std::string message, std::source_location Source)

        {

            // check if flag is in the list of active flags
            bool culled = true;
            for(LogFlag& testFlag : activeFlags) {
                if(flag == testFlag) {
                    culled = false;
                    break;
                }
            } 
            if(culled) return;

            std::string finalmessage = "";

            if (time)
                {
                    finalmessage = finalmessage + "[" +  "Not Implemented" + "] "; 
                }

            std::string componentTrace = "[" + id + ": " + component + "] ";
            finalmessage += componentTrace; // component is the section of the program (For example Mesh or Engine) that is calling the logger
                    
                std::string level = "";

            level = LogFlagStrings[flag];

            // level.append(7 - level.length(), ' ') pads out the level string with whitespace so every line is aligned the same
            // it looked weird though
            finalmessage = finalmessage + "[" + level + "] "; 

            finalmessage = finalmessage + message + " "; 

            if (additionaldetails)
                {
                    finalmessage = finalmessage  + "[Function: " + Source.function_name() + "]" + " " + "[Line: "  +  std::to_string(Source.line()) + "]" + " " + "[File: "  +  Source.file_name() + "]";
                }
   
            if (StandardOutput)
                {
                    std::cout << finalmessage << std::endl;
                }

            if (FileOutput)
                {
                    outfile << finalmessage << std::endl;
                }
            return;
        }