#include <stdio.h>
#include <windows.h>
#include "SharedFileIn.h"
#include "SharedFileOut.h"
#include "RaceTrackPositionInfo.h"

int initRaceTrackPositionInfo(FILE* logFile) {
	raceTrackPositionInfoFile = CreateFileMappingA(INVALID_HANDLE_VALUE, NULL, PAGE_READWRITE, 0, sizeof(RaceTrackPositionInfo_t), "Local\\KRPSMP_RaceTrackPositionInfo");

	if (raceTrackPositionInfoFile)
		raceTrackPositionInfoView = (RaceTrackPositionInfo_t*)MapViewOfFile(raceTrackPositionInfoFile, FILE_MAP_ALL_ACCESS, 0, 0, sizeof(RaceTrackPositionInfo_t));

	if (!raceTrackPositionInfoFile || !raceTrackPositionInfoView)
		return -1;

	raceTrackPositionInfoView->m_id = -1;

	if (logFile)
	fprintf(logFile, "KRPSMP: RaceTrackPosition Info initialised\n");

	return 1;
}

void deinitRaceTrackPositionInfo(FILE* logFile) {
	if (raceTrackPositionInfoFile || raceTrackPositionInfoView) {
		if (raceTrackPositionInfoView) {
			UnmapViewOfFile(raceTrackPositionInfoView);
			raceTrackPositionInfoView = NULL;
		}

		if (raceTrackPositionInfoFile)
		{
			CloseHandle(raceTrackPositionInfoFile);
			raceTrackPositionInfoFile = NULL;
		}

		if (logFile)
		fprintf(logFile, "KRPSMP: RaceTrackPosition Info deinitialised\n");
	}
}

void updateRaceTrackPositionInfo(FILE* logFile) {
	raceTrackPositionInfoView->m_id = raceTrackPositionInfoView->m_id + 1;
	FlushViewOfFile(raceTrackPositionInfoView, sizeof(RaceTrackPositionInfo_t));

	if (logFile)
	fprintf(logFile, "KRPSMP: RaceTrackPosition Info update\n");
}