#include <stdio.h>
#include <windows.h>
#include "SharedFileIn.h"
#include "SharedFileOut.h"
#include "CameraSet.h"

int connectCameraSet(FILE* logFile) {
	if (cameraSetFile || cameraSetView)
		disconnectCameraSet(logFile);

	cameraSetFile = CreateFileMappingA(INVALID_HANDLE_VALUE, NULL, PAGE_READWRITE, 0, sizeof(CameraSet_t), "Local\\KRPSMP_CameraSet");

	if(cameraSetFile)
		cameraSetView = (CameraSet_t*)MapViewOfFile(cameraSetFile, FILE_MAP_ALL_ACCESS, 0, 0, sizeof(CameraSet_t));

	if (!cameraSetFile || !cameraSetView)
		return -1;

	if (logFile)
		fprintf(logFile, "KRPSMP: Camera Set connected\n");

	return 1;
}

void disconnectCameraSet(FILE* logFile) {
	if (cameraSetFile || cameraSetView) {
		if (cameraSetView) {
			UnmapViewOfFile(cameraSetView);
			cameraSetView = NULL;
		}

		if (cameraSetFile) {
			CloseHandle(cameraSetFile);
			cameraSetFile = NULL;
		}

		if (logFile)
			fprintf(logFile, "KRPSMP: Camera Set disconnected\n");
	}
}

CameraSet_t* getCameraSet(FILE* logFile) {
	int connected = 1;
	if (!cameraSetFile || !cameraSetView)
		connected = connectCameraSet(logFile);

	if (connected == -1)
		return NULL;

	return cameraSetView;
}