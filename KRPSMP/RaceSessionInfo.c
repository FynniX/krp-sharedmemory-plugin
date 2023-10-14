#include <stdio.h>
#include <windows.h>
#include "SharedFileIn.h"
#include "SharedFileOut.h"
#include "RaceSessionInfo.h"

int initRaceSessionInfo(FILE* logFile) {
	raceSessionInfoFile = CreateFileMappingA(INVALID_HANDLE_VALUE, NULL, PAGE_READWRITE, 0, sizeof(RaceSessionInfo_t), "Local\\KRPSMP_RaceSessionInfo");

	if (raceSessionInfoFile)
		raceSessionInfoView = (RaceSessionInfo_t*)MapViewOfFile(raceSessionInfoFile, FILE_MAP_ALL_ACCESS, 0, 0, sizeof(RaceSessionInfo_t));

	if (!raceSessionInfoFile || !raceSessionInfoView)
		return -1;

	raceSessionInfoView->m_id = -1;

	if (logFile)
	fprintf(logFile, "KRPSMP: RaceSession Info initialised\n");

	return 1;
}

void deinitRaceSessionInfo(FILE* logFile) {
	if (raceSessionInfoFile || raceSessionInfoView) {
		if (raceSessionInfoView) {
			UnmapViewOfFile(raceSessionInfoView);
			raceSessionInfoView = NULL;
		}

		if (raceSessionInfoFile)
		{
			CloseHandle(raceSessionInfoFile);
			raceSessionInfoFile = NULL;
		}

		if (logFile)
		fprintf(logFile, "KRPSMP: RaceSession Info deinitialised\n");
	}
}

void updateRaceSessionInfo(FILE* logFile) {
	raceSessionInfoView->m_id = raceSessionInfoView->m_id + 1;
	FlushViewOfFile(raceSessionInfoView, sizeof(RaceSessionInfo_t));

	if (logFile)
	fprintf(logFile, "KRPSMP: RaceSession Info update\n");
}