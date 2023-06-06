#include <stdio.h>
#include <windows.h>
#include "SharedFileIn.h"
#include "SharedFileOut.h"
#include "KartTelemetryInfo.h"

int initKartTelemetryInfo(FILE* logFile) {
	kartTelemetryInfoFile = CreateFileMappingA(INVALID_HANDLE_VALUE, NULL, PAGE_READWRITE, 0, sizeof(KartTelemetryInfo_t), "Local\\KRPSMP_KartTelemetryInfo");

	if (kartTelemetryInfoFile)
		kartTelemetryInfoView = (KartTelemetryInfo_t*)MapViewOfFile(kartTelemetryInfoFile, FILE_MAP_ALL_ACCESS, 0, 0, sizeof(KartTelemetryInfo_t));

	if (!kartTelemetryInfoFile || !kartTelemetryInfoView)
		return -1;

	kartTelemetryInfoView->m_id = -1;

	fprintf(logFile, "KRPSMP: kartTelemetry Info initialised\n");

	return 1;
}

void deinitKartTelemetryInfo(FILE* logFile) {
	if (kartTelemetryInfoFile || kartTelemetryInfoView) {
		if (kartTelemetryInfoView) {
			UnmapViewOfFile(kartTelemetryInfoView);
			kartTelemetryInfoView = NULL;
		}

		if (kartTelemetryInfoFile)
		{
			CloseHandle(kartTelemetryInfoFile);
			kartTelemetryInfoFile = NULL;
		}

		fprintf(logFile, "KRPSMP: KartTelemetry Info deinitialised\n");
	}
}

void updateKartTelemetryInfo(FILE* logFile) {
	kartTelemetryInfoView->m_id = kartTelemetryInfoView->m_id + 1;
	FlushViewOfFile(kartTelemetryInfoView, sizeof(KartTelemetryInfo_t));

	fprintf(logFile, "KRPSMP: KartTelemetry Info update\n");
}