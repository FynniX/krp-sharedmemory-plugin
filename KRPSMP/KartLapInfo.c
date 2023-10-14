#include <stdio.h>
#include <windows.h>
#include "SharedFileIn.h"
#include "SharedFileOut.h"
#include "KartLapInfo.h"

int initKartLapInfo(FILE* logFile) {
	kartLapInfoFile = CreateFileMappingA(INVALID_HANDLE_VALUE, NULL, PAGE_READWRITE, 0, sizeof(KartLapInfo_t), "Local\\KRPSMP_KartLapInfo");

	if (kartLapInfoFile)
		kartLapInfoView = (KartLapInfo_t*)MapViewOfFile(kartLapInfoFile, FILE_MAP_ALL_ACCESS, 0, 0, sizeof(KartLapInfo_t));

	if (!kartLapInfoFile || !kartLapInfoView)
		return -1;

	kartLapInfoView->m_id = -1;

	if (logFile)
	fprintf(logFile, "KRPSMP: kartLap Info initialised\n");

	return 1;
}

void deinitKartLapInfo(FILE* logFile) {
	if (kartLapInfoFile || kartLapInfoView) {
		if (kartLapInfoView) {
			UnmapViewOfFile(kartLapInfoView);
			kartLapInfoView = NULL;
		}

		if (kartLapInfoFile)
		{
			CloseHandle(kartLapInfoFile);
			kartLapInfoFile = NULL;
		}

		if (logFile)
		fprintf(logFile, "KRPSMP: KartLap Info deinitialised\n");
	}
}

void updateKartLapInfo(FILE* logFile) {
	kartLapInfoView->m_id = kartLapInfoView->m_id + 1;
	FlushViewOfFile(kartLapInfoView, sizeof(KartLapInfo_t));

	if (logFile)
	fprintf(logFile, "KRPSMP: KartLap Info update\n");
}