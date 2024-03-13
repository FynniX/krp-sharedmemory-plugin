#include "Logger.h"

Logger::Logger(char *_szSavePath, bool enabled)
{
    if (!enabled)
        return;
    std::filesystem::path gameDir(_szSavePath);
    std::filesystem::path pluginDir("plugins\\krp-sharedmemory-plugin");
    std::filesystem::path file("log.txt");
    std::filesystem::create_directories(gameDir / pluginDir);
    logFile = fopen((gameDir / pluginDir / file).string().c_str(), "wt");
}

Logger::~Logger()
{
    if (!logFile)
        return;
    fclose(logFile);
}

void Logger::Log(std::string message)
{
    if (!logFile)
        return;
    fprintf_s(logFile, "%s\n", message.c_str());
}
