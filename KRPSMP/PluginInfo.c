#include <stdio.h>
#include <windows.h>
#include "SharedFileIn.h"
#include "SharedFileOut.h"
#include "PluginInfo.h"

int initPluginInfo(FILE* logFile) {
	pluginInfoFile = CreateFileMappingA(INVALID_HANDLE_VALUE, NULL, PAGE_READWRITE, 0, sizeof(PluginInfo_t), "Local\\KRPSMP_PluginInfo");

	if (pluginInfoFile)
		pluginInfoView = (PluginInfo_t*)MapViewOfFile(pluginInfoFile, FILE_MAP_ALL_ACCESS, 0, 0, sizeof(PluginInfo_t));

	if (!pluginInfoFile || !pluginInfoView)
		return -1;

	pluginInfoView->m_id = -1;

	fprintf(logFile, "KRPSMP: Plugin Info initialised\n");

	return 1;
}

void deinitPluginInfo(FILE* logFile) {
	if (pluginInfoFile || pluginInfoView) {
		if (pluginInfoView) {
			UnmapViewOfFile(pluginInfoView);
			pluginInfoView = NULL;
		}

		if (pluginInfoFile)
		{
			CloseHandle(pluginInfoFile);
			pluginInfoFile = NULL;
		}

		fprintf(logFile, "KRPSMP: Plugin Info deinitialised\n");
	}
}

void updatePluginInfo(FILE* logFile) {
	pluginInfoView->m_id = pluginInfoView->m_id + 1;
	FlushViewOfFile(pluginInfoView, sizeof(PluginInfo_t));

	fprintf(logFile, "KRPSMP: Plugin Info update\n");
}