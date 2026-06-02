#pragma once
#include <vector>
#include <chrono> // Tracking the time when the log function is called
#include <string>
#include <fstream>
#include <source_location> // Tracks a bunch information from the calling component ( )

#include "ConfigService.hpp"

// Goal - we want an centralized class the other components (like renderer or engine) can call on to log message

// Goal - use a enumerator to pass flags into the default constructor 

// create a log method that other sections will call that takes in the flag, message, and the calling component

// This is a monolithic logger so only one logger class will exist 

enum LogFlag{ Debug, Info, Warning, Error, Count };
static constexpr const char* LogFlagStrings[] = { "Debug", "Info", "Warning", "Error" };

class Logger 
{
    
    public:

        ~Logger();

        Logger(ConfigService* config, const std::string& loggerId);

        void log(std::string component, LogFlag flag, std::string message, std::source_location Source = std::source_location::current()); // Using the <source_location> 


    private: 

        bool StandardOutput;

        bool FileOutput;

        std::vector<LogFlag> activeFlags;

        bool additionaldetails;

        bool time;

        std::ofstream outfile;

        std::string id;

};

