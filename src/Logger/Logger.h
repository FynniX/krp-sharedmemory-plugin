#ifndef LOGGER_H
#define LOGGER_H

#include <iostream>
#include <filesystem>
#include <windows.h>

class Logger
{
private:
    FILE *logFile;
public:
    Logger(char *_szSavePath, bool enabled = true);
    ~Logger();
    void Log(std::string message);
};

#endif