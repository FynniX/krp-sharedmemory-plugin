#include <stdio.h>
#include <windows.h>
#include "SharedFileIn.h"
#include "SharedFileOut.h"
#include "RaceEntriesInfo.h"

int initRaceEntriesInfo(FILE* logFile) {
	raceEntriesInfoFile = CreateFileMappingA(INVALID_HANDLE_VALUE, NULL, PAGE_READWRITE, 0, sizeof(RaceEntriesInfo_t), "Local\\KRPSMP_RaceEntriesInfo");

	if (raceEntriesInfoFile)
		raceEntriesInfoView = (RaceEntriesInfo_t*)MapViewOfFile(raceEntriesInfoFile, FILE_MAP_ALL_ACCESS, 0, 0, sizeof(RaceEntriesInfo_t));

	if (!raceEntriesInfoFile || !raceEntriesInfoView)
		return -1;

	raceEntriesInfoView->m_id = -1;

	if (logFile)
	fprintf(logFile, "KRPSMP: RaceEntries Info initialised\n");

	return 1;
}

void deinitRaceEntriesInfo(FILE* logFile) {
	if (raceEntriesInfoFile || raceEntriesInfoView) {
		if (raceEntriesInfoView) {
			UnmapViewOfFile(raceEntriesInfoView);
			raceEntriesInfoView = NULL;
		}

		if (raceEntriesInfoFile)
		{
			CloseHandle(raceEntriesInfoFile);
			raceEntriesInfoFile = NULL;
		}

		if (logFile)
		fprintf(logFile, "KRPSMP: RaceEntries Info deinitialised\n");
	}
}

void updateRaceEntriesInfo(FILE* logFile) {
	raceEntriesInfoView->m_id = raceEntriesInfoView->m_id + 1;
	FlushViewOfFile(raceEntriesInfoView, sizeof(RaceEntriesInfo_t));

	if (logFile)
	fprintf(logFile, "KRPSMP: RaceEntries Info update\n");
}