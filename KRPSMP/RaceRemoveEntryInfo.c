#include <stdio.h>
#include <windows.h>
#include "SharedFileIn.h"
#include "SharedFileOut.h"
#include "RaceRemoveEntryInfo.h"

int initRaceRemoveEntryInfo(FILE* logFile) {
	raceRemoveEntryInfoFile = CreateFileMappingA(INVALID_HANDLE_VALUE, NULL, PAGE_READWRITE, 0, sizeof(RaceRemoveEntryInfo_t), "Local\\KRPSMP_RaceRemoveEntryInfo");

	if (raceRemoveEntryInfoFile)
		raceRemoveEntryInfoView = (RaceRemoveEntryInfo_t*)MapViewOfFile(raceRemoveEntryInfoFile, FILE_MAP_ALL_ACCESS, 0, 0, sizeof(RaceRemoveEntryInfo_t));

	if (!raceRemoveEntryInfoFile || !raceRemoveEntryInfoView)
		return -1;

	raceRemoveEntryInfoView->m_id = -1;

	if (logFile)
	fprintf(logFile, "KRPSMP: RaceRemoveEntry Info initialised\n");

	return 1;
}

void deinitRaceRemoveEntryInfo(FILE* logFile) {
	if (raceRemoveEntryInfoFile || raceRemoveEntryInfoView) {
		if (raceRemoveEntryInfoView) {
			UnmapViewOfFile(raceRemoveEntryInfoView);
			raceRemoveEntryInfoView = NULL;
		}

		if (raceRemoveEntryInfoFile)
		{
			CloseHandle(raceRemoveEntryInfoFile);
			raceRemoveEntryInfoFile = NULL;
		}

		if (logFile)
		fprintf(logFile, "KRPSMP: RaceRemoveEntry Info deinitialised\n");
	}
}

void updateRaceRemoveEntryInfo(FILE* logFile) {
	raceRemoveEntryInfoView->m_id = raceRemoveEntryInfoView->m_id + 1;
	FlushViewOfFile(raceRemoveEntryInfoView, sizeof(RaceRemoveEntryInfo_t));

	if (logFile)
	fprintf(logFile, "KRPSMP: RaceRemoveEntry Info update\n");
}