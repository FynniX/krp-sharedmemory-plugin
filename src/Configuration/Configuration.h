#ifndef CONFIGURATION_H
#define CONFIGURATION_H





#include <iostream>
#include <string>
#include <filesystem>
#include <direct.h>
#include <windows.h>
#include "SharedFileOut.h"

typedef struct
{
    int logging;
    int enabled;
    ConfigurationRate_t rate;
} Configuration_t;

class Configuration
{
private:
    std::filesystem::path m_szSavePath;
    Configuration_t configuration = {0};
    void Save();
    void Read();

public:
    Configuration(char *_szSavePath);
    inline Configuration_t GetConfiguration() { return configuration; }
    void SaveConfiguration(Configuration_t newConfiguration);
    std::string GetPrintableConfiguration();
};

#endif