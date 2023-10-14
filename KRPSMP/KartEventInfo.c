#include <stdio.h>
#include <windows.h>
#include "SharedFileIn.h"
#include "SharedFileOut.h"
#include "KartEventInfo.h"

int initKartEventInfo(FILE* logFile) {
	kartEventInfoFile = CreateFileMappingA(INVALID_HANDLE_VALUE, NULL, PAGE_READWRITE, 0, sizeof(KartEventInfo_t), "Local\\KRPSMP_KartEventInfo");

	if (kartEventInfoFile)
		kartEventInfoView = (KartEventInfo_t*)MapViewOfFile(kartEventInfoFile, FILE_MAP_ALL_ACCESS, 0, 0, sizeof(KartEventInfo_t));

	if (!kartEventInfoFile || !kartEventInfoView)
		return -1;

	kartEventInfoView->m_id = -1;

	if(logFile)
	fprintf(logFile, "KRPSMP: kartEvent Info initialised\n");

	return 1;
}

void deinitKartEventInfo(FILE* logFile) {
	if (kartEventInfoFile || kartEventInfoView) {
		if (kartEventInfoView) {
			UnmapViewOfFile(kartEventInfoView);
			kartEventInfoView = NULL;
		}

		if (kartEventInfoFile)
		{
			CloseHandle(kartEventInfoFile);
			kartEventInfoFile = NULL;
		}

		if (logFile)
		fprintf(logFile, "KRPSMP: KartEvent Info deinitialised\n");
	}
}

void updateKartEventInfo(FILE* logFile) {
	kartEventInfoView->m_id = kartEventInfoView->m_id + 1;
	FlushViewOfFile(kartEventInfoView, sizeof(KartEventInfo_t));

	if (logFile)
	fprintf(logFile, "KRPSMP: KartEvent Info update\n");
}