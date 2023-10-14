#include <stdio.h>
#include <windows.h>
#include "SharedFileIn.h"
#include "SharedFileOut.h"
#include "RaceSessionStateInfo.h"

int initRaceSessionStateInfo(FILE* logFile) {
	raceSessionStateInfoFile = CreateFileMappingA(INVALID_HANDLE_VALUE, NULL, PAGE_READWRITE, 0, sizeof(RaceSessionStateInfo_t), "Local\\KRPSMP_RaceSessionStateInfo");

	if (raceSessionStateInfoFile)
		raceSessionStateInfoView = (RaceSessionStateInfo_t*)MapViewOfFile(raceSessionStateInfoFile, FILE_MAP_ALL_ACCESS, 0, 0, sizeof(RaceSessionStateInfo_t));

	if (!raceSessionStateInfoFile || !raceSessionStateInfoView)
		return -1;

	raceSessionStateInfoView->m_id = -1;

	if (logFile)
	fprintf(logFile, "KRPSMP: RaceSessionState Info initialised\n");

	return 1;
}

void deinitRaceSessionStateInfo(FILE* logFile) {
	if (raceSessionStateInfoFile || raceSessionStateInfoView) {
		if (raceSessionStateInfoView) {
			UnmapViewOfFile(raceSessionStateInfoView);
			raceSessionStateInfoView = NULL;
		}

		if (raceSessionStateInfoFile)
		{
			CloseHandle(raceSessionStateInfoFile);
			raceSessionStateInfoFile = NULL;
		}

		if (logFile)
		fprintf(logFile, "KRPSMP: RaceSessionState Info deinitialised\n");
	}
}

void updateRaceSessionStateInfo(FILE* logFile) {
	raceSessionStateInfoView->m_id = raceSessionStateInfoView->m_id + 1;
	FlushViewOfFile(raceSessionStateInfoView, sizeof(RaceSessionStateInfo_t));

	if (logFile)
	fprintf(logFile, "KRPSMP: RaceSessionState Info update\n");
}