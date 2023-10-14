#include <stdio.h>
#include <windows.h>
#include "SharedFileIn.h"
#include "SharedFileOut.h"
#include "RaceEventInfo.h"

int initRaceEventInfo(FILE* logFile) {
	raceEventInfoFile = CreateFileMappingA(INVALID_HANDLE_VALUE, NULL, PAGE_READWRITE, 0, sizeof(RaceEventInfo_t), "Local\\KRPSMP_RaceEventInfo");

	if (raceEventInfoFile)
		raceEventInfoView = (RaceEventInfo_t*)MapViewOfFile(raceEventInfoFile, FILE_MAP_ALL_ACCESS, 0, 0, sizeof(RaceEventInfo_t));

	if (!raceEventInfoFile || !raceEventInfoView)
		return -1;

	raceEventInfoView->m_id = -1;

	if (logFile)
	fprintf(logFile, "KRPSMP: RaceEvent Info initialised\n");

	return 1;
}

void deinitRaceEventInfo(FILE* logFile) {
	if (raceEventInfoFile || raceEventInfoView) {
		if (raceEventInfoView) {
			UnmapViewOfFile(raceEventInfoView);
			raceEventInfoView = NULL;
		}

		if (raceEventInfoFile)
		{
			CloseHandle(raceEventInfoFile);
			raceEventInfoFile = NULL;
		}

		if (logFile)
		fprintf(logFile, "KRPSMP: RaceEvent Info deinitialised\n");
	}
}

void updateRaceEventInfo(FILE* logFile) {
	raceEventInfoView->m_id = raceEventInfoView->m_id + 1;
	FlushViewOfFile(raceEventInfoView, sizeof(RaceEventInfo_t));

	if (logFile)
	fprintf(logFile, "KRPSMP: RaceEvent Info update\n");
}