#include <stdio.h>
#include <windows.h>
#include "SharedFileIn.h"
#include "SharedFileOut.h"
#include "RaceSplitInfo.h"

int initRaceSplitInfo(FILE* logFile) {
	raceSplitInfoFile = CreateFileMappingA(INVALID_HANDLE_VALUE, NULL, PAGE_READWRITE, 0, sizeof(RaceSplitInfo_t), "Local\\KRPSMP_RaceSplitInfo");

	if (raceSplitInfoFile)
		raceSplitInfoView = (RaceSplitInfo_t*)MapViewOfFile(raceSplitInfoFile, FILE_MAP_ALL_ACCESS, 0, 0, sizeof(RaceSplitInfo_t));

	if (!raceSplitInfoFile || !raceSplitInfoView)
		return -1;

	raceSplitInfoView->m_id = -1;

	if (logFile)
	fprintf(logFile, "KRPSMP: RaceSplit Info initialised\n");

	return 1;
}

void deinitRaceSplitInfo(FILE* logFile) {
	if (raceSplitInfoFile || raceSplitInfoView) {
		if (raceSplitInfoView) {
			UnmapViewOfFile(raceSplitInfoView);
			raceSplitInfoView = NULL;
		}

		if (raceSplitInfoFile)
		{
			CloseHandle(raceSplitInfoFile);
			raceSplitInfoFile = NULL;
		}

		if (logFile)
		fprintf(logFile, "KRPSMP: RaceSplit Info deinitialised\n");
	}
}

void updateRaceSplitInfo(FILE* logFile) {
	raceSplitInfoView->m_id = raceSplitInfoView->m_id + 1;
	FlushViewOfFile(raceSplitInfoView, sizeof(RaceSplitInfo_t));

	if (logFile)
	fprintf(logFile, "KRPSMP: RaceSplit Info update\n");
}