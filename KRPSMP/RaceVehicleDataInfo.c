#include <stdio.h>
#include <windows.h>
#include "SharedFileIn.h"
#include "SharedFileOut.h"
#include "RaceVehicleDataInfo.h"

int initRaceVehicleDataInfo(FILE* logFile) {
	raceVehicleDataInfoFile = CreateFileMappingA(INVALID_HANDLE_VALUE, NULL, PAGE_READWRITE, 0, sizeof(RaceVehicleDataInfo_t), "Local\\KRPSMP_RaceVehicleDataInfo");

	if (raceVehicleDataInfoFile)
		raceVehicleDataInfoView = (RaceVehicleDataInfo_t*)MapViewOfFile(raceVehicleDataInfoFile, FILE_MAP_ALL_ACCESS, 0, 0, sizeof(RaceVehicleDataInfo_t));

	if (!raceVehicleDataInfoFile || !raceVehicleDataInfoView)
		return -1;

	raceVehicleDataInfoView->m_id = -1;

	fprintf(logFile, "KRPSMP: RaceVehicleData Info initialised\n");

	return 1;
}

void deinitRaceVehicleDataInfo(FILE* logFile) {
	if (raceVehicleDataInfoFile || raceVehicleDataInfoView) {
		if (raceVehicleDataInfoView) {
			UnmapViewOfFile(raceVehicleDataInfoView);
			raceVehicleDataInfoView = NULL;
		}

		if (raceVehicleDataInfoFile)
		{
			CloseHandle(raceVehicleDataInfoFile);
			raceVehicleDataInfoFile = NULL;
		}

		fprintf(logFile, "KRPSMP: RaceVehicleData Info deinitialised\n");
	}
}

void updateRaceVehicleDataInfo(FILE* logFile) {
	raceVehicleDataInfoView->m_id = raceVehicleDataInfoView->m_id + 1;
	FlushViewOfFile(raceVehicleDataInfoView, sizeof(RaceVehicleDataInfo_t));

	fprintf(logFile, "KRPSMP: RaceVehicleData Info update\n");
}