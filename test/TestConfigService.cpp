#include <gmock/gmock.h>

#include <ConfigService.hpp>

#include <filesystem>
#include <fstream>

TEST(ConfigService, ParsesLoggerConfigById) {
    ConfigService config;

    const char* configText = R"(
Loggers = (
    {
        Id = "Engine";
        FlagsEnabled = (
            "Debug",
            "Info",
            "Warning",
            "Error"
        );
        ShowTime = true;
        ShowSourceTrace = true;
        CoutEnabled = true;
        FileEnabled = true;
        FilePath = "Logs/Engine.log";
    },
    {
        Id = "Renderer";
        FlagsEnabled = (
            "Error"
        );
        ShowTime = false;
        ShowSourceTrace = false;
        CoutEnabled = false;
        FileEnabled = true;
        FilePath = "Logs/Renderer.log";
    }
);
)";

    EXPECT_TRUE(config.loadFromString(configText));

    const auto loggerConfig = config.getLoggerConfig("Engine");

    ASSERT_TRUE(loggerConfig.has_value());
    EXPECT_EQ(loggerConfig->id, "Engine");
    EXPECT_THAT(loggerConfig->flagsEnabled,
                testing::ElementsAre("Debug", "Info", "Warning", "Error"));
    EXPECT_TRUE(loggerConfig->showTime);
    EXPECT_TRUE(loggerConfig->showSourceTrace);
    EXPECT_TRUE(loggerConfig->coutEnabled);
    EXPECT_TRUE(loggerConfig->fileEnabled);
    EXPECT_EQ(loggerConfig->filePath, "Logs/Engine.log");
}

TEST(ConfigService, ReturnsEmptyWhenLoggerIdIsMissing) {
    ConfigService config;

    const char* configText = R"(
Loggers = (
    {
        Id = "Engine";
        FlagsEnabled = (
            "Error"
        );
        ShowTime = true;
        ShowSourceTrace = true;
        CoutEnabled = true;
        FileEnabled = false;
        FilePath = "";
    }
);
)";

    ASSERT_TRUE(config.loadFromString(configText));

    EXPECT_FALSE(config.getLoggerConfig("Renderer").has_value());
}

TEST(ConfigService, LoadsLoggerConfigFromFile) {
    const auto configPath = std::filesystem::temp_directory_path() / "maiden-test-config.cfg";

    {
        std::ofstream configFile(configPath);
        configFile << R"(
Loggers = (
    {
        Id = "Engine";
        FlagsEnabled = (
            "Info",
            "Error"
        );
        ShowTime = true;
        ShowSourceTrace = false;
        CoutEnabled = true;
        FileEnabled = false;
        FilePath = "";
    }
);
)";
    }

    ConfigService config;

    ASSERT_TRUE(config.loadFromFile(configPath.string())) << config.lastError();

    const auto loggerConfig = config.getLoggerConfig("Engine");

    ASSERT_TRUE(loggerConfig.has_value());
    EXPECT_THAT(loggerConfig->flagsEnabled, testing::ElementsAre("Info", "Error"));
    EXPECT_TRUE(loggerConfig->showTime);
    EXPECT_FALSE(loggerConfig->showSourceTrace);
    EXPECT_TRUE(loggerConfig->coutEnabled);
    EXPECT_FALSE(loggerConfig->fileEnabled);
    EXPECT_TRUE(loggerConfig->filePath.empty());

    std::filesystem::remove(configPath);
}
