#include "ConfigService.hpp"

#include <iostream>
#include <exception>

bool ConfigService::loadFromFile(const std::string& filePath) {
    try {
        config_.clear();
        config_.readFile(filePath.c_str());
        lastError_.clear();
        return true;
    } catch (const libconfig::FileIOException&) {
        lastError_ = "Unable to read config file: " + filePath;
    } catch (const libconfig::ParseException& error) {
        lastError_ = std::string("Parse error in ") + error.getFile() + ":" +
                     std::to_string(error.getLine()) + " - " + error.getError();
    } catch (const std::exception& error) {
        lastError_ = error.what();
    }

    return false;
}

bool ConfigService::loadFromString(const std::string& configText) {
    try {
        config_.clear();
        config_.readString(configText.c_str());
        lastError_.clear();
        return true;
    } catch (const libconfig::ParseException& error) {
        lastError_ = std::string("Parse error in config text:") +
                     std::to_string(error.getLine()) + " - " + error.getError();
    } catch (const std::exception& error) {
        lastError_ = error.what();
    }

    return false;
}

std::optional<LoggerConfig> ConfigService::getLoggerConfig(const std::string& id) const {
    try {
        const libconfig::Setting& loggers = config_.lookup("Loggers");
        for (int index = 0; index < loggers.getLength(); ++index) {
            const libconfig::Setting& loggerSetting = loggers[index];

            std::string loggerId;
            if (!loggerSetting.lookupValue("Id", loggerId) || loggerId != id) {
                continue;
            }

            return parseLoggerConfig(loggerSetting);
        }
    } catch (const libconfig::SettingException& ex) {
        std::cout << "libconfig setting exception: " << ex.what() << std::endl; 
        return std::nullopt;
    }

    return std::nullopt;
}

const std::string& ConfigService::lastError() const {
    return lastError_;
}

std::optional<LoggerConfig> ConfigService::parseLoggerConfig(
    const libconfig::Setting& loggerSetting) const {
    LoggerConfig loggerConfig;

    if (!loggerSetting.lookupValue("Id", loggerConfig.id)) {
        return std::nullopt;
    }

    try {
        const libconfig::Setting& flags = loggerSetting.lookup("FlagsEnabled");
        for (int index = 0; index < flags.getLength(); ++index) {
            loggerConfig.flagsEnabled.push_back(static_cast<const char*>(flags[index]));
        }
    } catch (const libconfig::SettingException&) {
        loggerConfig.flagsEnabled.clear();
    }

    loggerSetting.lookupValue("ShowTime", loggerConfig.showTime);
    loggerSetting.lookupValue("ShowSourceTrace", loggerConfig.showSourceTrace);
    loggerSetting.lookupValue("CoutEnabled", loggerConfig.coutEnabled);
    loggerSetting.lookupValue("FileEnabled", loggerConfig.fileEnabled);
    loggerSetting.lookupValue("FilePath", loggerConfig.filePath);

    return loggerConfig;
}
