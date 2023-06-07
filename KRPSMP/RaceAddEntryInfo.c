#include <stdio.h>
#include <windows.h>
#include "SharedFileIn.h"
#include "SharedFileOut.h"
#include "RaceAddEntryInfo.h"

int initRaceAddEntryInfo(FILE* logFile) {
	raceAddEntryInfoFile = CreateFileMappingA(INVALID_HANDLE_VALUE, NULL, PAGE_READWRITE, 0, sizeof(RaceAddEntryInfo_t), "Local\\KRPSMP_RaceAddEntryInfo");

	if (raceAddEntryInfoFile)
		raceAddEntryInfoView = (RaceAddEntryInfo_t*)MapViewOfFile(raceAddEntryInfoFile, FILE_MAP_ALL_ACCESS, 0, 0, sizeof(RaceAddEntryInfo_t));

	if (!raceAddEntryInfoFile || !raceAddEntryInfoView)
		return -1;

	raceAddEntryInfoView->m_id = -1;

	fprintf(logFile, "KRPSMP: RaceAddEntry Info initialised\n");

	return 1;
}

void deinitRaceAddEntryInfo(FILE* logFile) {
	if (raceAddEntryInfoFile || raceAddEntryInfoView) {
		if (raceAddEntryInfoView) {
			UnmapViewOfFile(raceAddEntryInfoView);
			raceAddEntryInfoView = NULL;
		}

		if (raceAddEntryInfoFile)
		{
			CloseHandle(raceAddEntryInfoFile);
			raceAddEntryInfoFile = NULL;
		}

		fprintf(logFile, "KRPSMP: RaceAddEntry Info deinitialised\n");
	}
}

void updateRaceAddEntryInfo(FILE* logFile) {
	raceAddEntryInfoView->m_id = raceAddEntryInfoView->m_id + 1;
	FlushViewOfFile(raceAddEntryInfoView, sizeof(RaceAddEntryInfo_t));

	fprintf(logFile, "KRPSMP: RaceAddEntry Info update\n");
}