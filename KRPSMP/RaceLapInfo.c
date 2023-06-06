#include <stdio.h>
#include <windows.h>
#include "SharedFileIn.h"
#include "SharedFileOut.h"
#include "RaceLapInfo.h"

int initRaceLapInfo(FILE* logFile) {
	raceLapInfoFile = CreateFileMappingA(INVALID_HANDLE_VALUE, NULL, PAGE_READWRITE, 0, sizeof(RaceLapInfo_t), "Local\\KRPSMP_RaceLapInfo");

	if (raceLapInfoFile)
		raceLapInfoFile = (RaceLapInfo_t*)MapViewOfFile(raceLapInfoFile, FILE_MAP_ALL_ACCESS, 0, 0, sizeof(RaceLapInfo_t));

	if (!raceLapInfoFile || !raceLapInfoView)
		return -1;

	raceLapInfoView->m_id = -1;

	fprintf(logFile, "KRPSMP: RaceLap Info initialised\n");

	return 1;
}

void deinitRaceLapInfo(FILE* logFile) {
	if (raceLapInfoFile || raceLapInfoView) {
		if (raceLapInfoView) {
			UnmapViewOfFile(raceLapInfoView);
			raceLapInfoView = NULL;
		}

		if (raceLapInfoFile)
		{
			CloseHandle(raceLapInfoFile);
			raceLapInfoFile = NULL;
		}

		fprintf(logFile, "KRPSMP: RaceLap Info deinitialised\n");
	}
}

void updateRaceLapInfo(FILE* logFile) {
	raceLapInfoView->m_id = raceLapInfoView->m_id + 1;
	FlushViewOfFile(raceLapInfoView, sizeof(RaceLapInfo_t));

	fprintf(logFile, "KRPSMP: RaceLap Info update\n");
}