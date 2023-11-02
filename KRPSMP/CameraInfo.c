#include <stdio.h>
#include <windows.h>
#include "SharedFileIn.h"
#include "SharedFileOut.h"
#include "CameraInfo.h"

int initCameraInfo(FILE* logFile) {
	cameraInfoFile = CreateFileMappingA(INVALID_HANDLE_VALUE, NULL, PAGE_READWRITE, 0, sizeof(CameraInfo_t), "Local\\KRPSMP_CameraInfo");

	if (cameraInfoFile)
		cameraInfoView = (CameraInfo_t*)MapViewOfFile(cameraInfoFile, FILE_MAP_ALL_ACCESS, 0, 0, sizeof(CameraInfo_t));

	if (!cameraInfoFile || !cameraInfoView)
		return -1;

	cameraInfoView->m_id = -1;

	if (logFile)
		fprintf(logFile, "KRPSMP: Camera Info initialised\n");

	return 1;
}

void deinitCameraInfo(FILE* logFile) {
	if (cameraInfoFile || cameraInfoView) {
		if (cameraInfoView) {
			UnmapViewOfFile(cameraInfoView);
			cameraInfoView = NULL;
		}

		if (cameraInfoFile)
		{
			CloseHandle(cameraInfoFile);
			cameraInfoFile = NULL;
		}

		if (logFile)
			fprintf(logFile, "KRPSMP: Camera Info deinitialised\n");
	}
}

void updateCameraInfo(FILE* logFile) {
	cameraInfoView->m_id = cameraInfoView->m_id + 1;
	FlushViewOfFile(cameraInfoView, sizeof(CameraInfo_t));

	if (logFile)
		fprintf(logFile, "KRPSMP: Camera Info update\n");
}