#pragma once

#include <libconfig.h++>

#include <optional>
#include <string>
#include <vector>

struct LoggerConfig {
    std::string id;
    std::vector<std::string> flagsEnabled;
    bool showTime = false;
    bool showSourceTrace = false;
    bool coutEnabled = false;
    bool fileEnabled = false;
    std::string filePath;
};

class ConfigService {
public:
    bool loadFromFile(const std::string& filePath);
    bool loadFromString(const std::string& configText);

    std::optional<LoggerConfig> getLoggerConfig(const std::string& id) const;
    const std::string& lastError() const;

private:
    std::optional<LoggerConfig> parseLoggerConfig(const libconfig::Setting& loggerSetting) const;

    libconfig::Config config_;
    std::string lastError_;
};
