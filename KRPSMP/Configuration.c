#include <stdio.h>
#include <direct.h>
#include <windows.h>
#include "Configuration.h"

Configuration_t getConfiguration() {
	Configuration_t configuration = {0};

	configuration.enable = GetPrivateProfileInt(L"params", L"enable", 0, L"./krpsmp.ini");
	configuration.rate = GetPrivateProfileInt(L"params", L"rate", 3, L"./krpsmp.ini");

	return configuration;
}