#include <stdio.h>
#include <windows.h>
#include "SharedFileIn.h"
#include "SharedFileOut.h"
#include "RaceCommunicationInfo.h"

int initRaceCommunicationInfo(FILE* logFile) {
	raceCommunicationInfoFile = CreateFileMappingA(INVALID_HANDLE_VALUE, NULL, PAGE_READWRITE, 0, sizeof(RaceCommunicationInfo_t), "Local\\KRPSMP_RaceCommunicationInfo");

	if (raceCommunicationInfoFile)
		raceCommunicationInfoFile = (RaceCommunicationInfo_t*)MapViewOfFile(raceCommunicationInfoFile, FILE_MAP_ALL_ACCESS, 0, 0, sizeof(RaceCommunicationInfo_t));

	if (!raceCommunicationInfoFile || !raceCommunicationInfoView)
		return -1;

	raceCommunicationInfoView->m_id = -1;

	fprintf(logFile, "KRPSMP: RaceCommunication Info initialised\n");

	return 1;
}

void deinitRaceCommunicationInfo(FILE* logFile) {
	if (raceCommunicationInfoFile || raceCommunicationInfoView) {
		if (raceCommunicationInfoView) {
			UnmapViewOfFile(raceCommunicationInfoView);
			raceCommunicationInfoView = NULL;
		}

		if (raceCommunicationInfoFile)
		{
			CloseHandle(raceCommunicationInfoFile);
			raceCommunicationInfoFile = NULL;
		}

		fprintf(logFile, "KRPSMP: RaceCommunication Info deinitialised\n");
	}
}

void updateRaceCommunicationInfo(FILE* logFile) {
	raceCommunicationInfoView->m_id = raceCommunicationInfoView->m_id + 1;
	FlushViewOfFile(raceCommunicationInfoView, sizeof(RaceCommunicationInfo_t));

	fprintf(logFile, "KRPSMP: RaceCommunication Info update\n");
}