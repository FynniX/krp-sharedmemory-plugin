#pragma once

HANDLE cameraSetFile;
CameraSet_t* cameraSetView;

int connectCameraSet(FILE* logFile);
void disconnectCameraSet(FILE* logFile);
CameraSet_t* getCameraSet(FILE* logFile);