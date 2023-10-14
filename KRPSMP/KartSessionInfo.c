#include <stdio.h>
#include <windows.h>
#include "SharedFileIn.h"
#include "SharedFileOut.h"
#include "KartSessionInfo.h"

int initKartSessionInfo(FILE* logFile) {
	kartSessionInfoFile = CreateFileMappingA(INVALID_HANDLE_VALUE, NULL, PAGE_READWRITE, 0, sizeof(KartSessionInfo_t), "Local\\KRPSMP_KartSessionInfo");

	if (kartSessionInfoFile)
		kartSessionInfoView = (KartSessionInfo_t*)MapViewOfFile(kartSessionInfoFile, FILE_MAP_ALL_ACCESS, 0, 0, sizeof(KartSessionInfo_t));

	if (!kartSessionInfoFile || !kartSessionInfoView)
		return -1;

	kartSessionInfoView->m_id = -1;

	if (logFile)
	fprintf(logFile, "KRPSMP: kartSession Info initialised\n");

	return 1;
}

void deinitKartSessionInfo(FILE* logFile) {
	if (kartSessionInfoFile || kartSessionInfoView) {
		if (kartSessionInfoView) {
			UnmapViewOfFile(kartSessionInfoView);
			kartSessionInfoView = NULL;
		}

		if (kartSessionInfoFile)
		{
			CloseHandle(kartSessionInfoFile);
			kartSessionInfoFile = NULL;
		}

		if (logFile)
		fprintf(logFile, "KRPSMP: KartSession Info deinitialised\n");
	}
}

void updateKartSessionInfo(FILE* logFile) {
	kartSessionInfoView->m_id = kartSessionInfoView->m_id + 1;
	FlushViewOfFile(kartSessionInfoView, sizeof(KartSessionInfo_t));

	if (logFile)
	fprintf(logFile, "KRPSMP: KartSession Info update\n");
}