#include "Configuration.h"

Configuration::Configuration(char *_szSavePath)
{
    std::filesystem::path gameDir(_szSavePath);
    std::filesystem::path pluginDir("plugins\\krp-sharedmemory-plugin");
    std::filesystem::path file("config.ini");
    std::filesystem::create_directories(gameDir / pluginDir);
    m_szSavePath = gameDir / pluginDir / file;
    Read();
    Save();
}

void Configuration::SaveConfiguration(Configuration_t newConfiguration)
{
    configuration = newConfiguration;
    Save();
}

std::string Configuration::GetPrintableConfiguration()
{
    return "Configuration: Logging: " + std::to_string(configuration.logging) + ", Enabled: " + std::to_string(configuration.enabled) + ", Rate: " + std::to_string(configuration.rate);
}

void Configuration::Save()
{
    WritePrivateProfileStringA("Configuration", "Logging", std::to_string(configuration.logging).c_str(), m_szSavePath.string().c_str());
    WritePrivateProfileStringA("Configuration", "Enabled", std::to_string(configuration.enabled).c_str(), m_szSavePath.string().c_str());
    WritePrivateProfileStringA("Configuration", "Rate", std::to_string(configuration.rate).c_str(), m_szSavePath.string().c_str());
}

void Configuration::Read()
{
    configuration.logging = GetPrivateProfileIntA("Configuration", "Logging", 0, m_szSavePath.string().c_str());
    configuration.enabled = GetPrivateProfileIntA("Configuration", "Enabled", 1, m_szSavePath.string().c_str());
    configuration.rate = (ConfigurationRate_t)GetPrivateProfileIntA("Configuration", "Rate", ConfigurationRate_t::HUNDRED, m_szSavePath.string().c_str());
}