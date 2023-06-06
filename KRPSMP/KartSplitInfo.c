#include <stdio.h>
#include <windows.h>
#include "SharedFileIn.h"
#include "SharedFileOut.h"
#include "KartSplitInfo.h"

int initKartSplitInfo(FILE* logFile) {
	kartSplitInfoFile = CreateFileMappingA(INVALID_HANDLE_VALUE, NULL, PAGE_READWRITE, 0, sizeof(KartSplitInfo_t), "Local\\KRPSMP_KartSplitInfo");

	if (kartSplitInfoFile)
		kartSplitInfoView = (KartSplitInfo_t*)MapViewOfFile(kartSplitInfoFile, FILE_MAP_ALL_ACCESS, 0, 0, sizeof(KartSplitInfo_t));

	if (!kartSplitInfoFile || !kartSplitInfoView)
		return -1;

	kartSplitInfoView->m_id = -1;

	fprintf(logFile, "KRPSMP: kartSplit Info initialised\n");

	return 1;
}

void deinitKartSplitInfo(FILE* logFile) {
	if (kartSplitInfoFile || kartSplitInfoView) {
		if (kartSplitInfoView) {
			UnmapViewOfFile(kartSplitInfoView);
			kartSplitInfoView = NULL;
		}

		if (kartSplitInfoFile)
		{
			CloseHandle(kartSplitInfoFile);
			kartSplitInfoFile = NULL;
		}

		fprintf(logFile, "KRPSMP: KartSplit Info deinitialised\n");
	}
}

void updateKartSplitInfo(FILE* logFile) {
	kartSplitInfoView->m_id = kartSplitInfoView->m_id + 1;
	FlushViewOfFile(kartSplitInfoView, sizeof(KartSplitInfo_t));

	fprintf(logFile, "KRPSMP: KartSplit Info update\n");
}