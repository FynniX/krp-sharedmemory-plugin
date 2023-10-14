#include <stdio.h>
#include <windows.h>
#include "SharedFileIn.h"
#include "SharedFileOut.h"
#include "RaceClassificationInfo.h"

int initRaceClassificationInfo(FILE* logFile) {
	raceClassificationInfoFile = CreateFileMappingA(INVALID_HANDLE_VALUE, NULL, PAGE_READWRITE, 0, sizeof(RaceClassificationInfo_t), "Local\\KRPSMP_RaceClassificationInfo");

	if (raceClassificationInfoFile)
		raceClassificationInfoView = (RaceClassificationInfo_t*)MapViewOfFile(raceClassificationInfoFile, FILE_MAP_ALL_ACCESS, 0, 0, sizeof(RaceClassificationInfo_t));

	if (!raceClassificationInfoFile || !raceClassificationInfoView)
		return -1;

	raceClassificationInfoView->m_id = -1;

	if (logFile)
	fprintf(logFile, "KRPSMP: RaceClassification Info initialised\n");

	return 1;
}

void deinitRaceClassificationInfo(FILE* logFile) {
	if (raceClassificationInfoFile || raceClassificationInfoView) {
		if (raceClassificationInfoView) {
			UnmapViewOfFile(raceClassificationInfoView);
			raceClassificationInfoView = NULL;
		}

		if (raceClassificationInfoFile)
		{
			CloseHandle(raceClassificationInfoFile);
			raceClassificationInfoFile = NULL;
		}

		if (logFile)
		fprintf(logFile, "KRPSMP: RaceClassification Info deinitialised\n");
	}
}

void updateRaceClassificationInfo(FILE* logFile) {
	raceClassificationInfoView->m_id = raceClassificationInfoView->m_id + 1;
	FlushViewOfFile(raceClassificationInfoView, sizeof(RaceClassificationInfo_t));

	if (logFile)
	fprintf(logFile, "KRPSMP: RaceClassification Info update\n");
}