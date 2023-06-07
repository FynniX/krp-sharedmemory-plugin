#include <stdio.h>
#include <windows.h>
#include "SharedFileIn.h"
#include "SharedFileOut.h"
#include "RaceSpeedInfo.h"

int initRaceSpeedInfo(FILE* logFile) {
	raceSpeedInfoFile = CreateFileMappingA(INVALID_HANDLE_VALUE, NULL, PAGE_READWRITE, 0, sizeof(RaceSpeedInfo_t), "Local\\KRPSMP_RaceSpeedInfo");

	if (raceSpeedInfoFile)
		raceSpeedInfoView = (RaceSpeedInfo_t*)MapViewOfFile(raceSpeedInfoFile, FILE_MAP_ALL_ACCESS, 0, 0, sizeof(RaceSpeedInfo_t));

	if (!raceSpeedInfoFile || !raceSpeedInfoView)
		return -1;

	raceSpeedInfoView->m_id = -1;

	fprintf(logFile, "KRPSMP: RaceSpeed Info initialised\n");

	return 1;
}

void deinitRaceSpeedInfo(FILE* logFile) {
	if (raceSpeedInfoFile || raceSpeedInfoView) {
		if (raceSpeedInfoView) {
			UnmapViewOfFile(raceSpeedInfoView);
			raceSpeedInfoView = NULL;
		}

		if (raceSpeedInfoFile)
		{
			CloseHandle(raceSpeedInfoFile);
			raceSpeedInfoFile = NULL;
		}

		fprintf(logFile, "KRPSMP: RaceSpeed Info deinitialised\n");
	}
}

void updateRaceSpeedInfo(FILE* logFile) {
	raceSpeedInfoView->m_id = raceSpeedInfoView->m_id + 1;
	FlushViewOfFile(raceSpeedInfoView, sizeof(RaceSpeedInfo_t));

	fprintf(logFile, "KRPSMP: RaceSpeed Info update\n");
}