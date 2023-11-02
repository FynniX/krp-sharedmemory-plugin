#pragma once

HANDLE cameraInfoFile;
CameraInfo_t* cameraInfoView;

int initCameraInfo(FILE*);
void deinitCameraInfo(FILE*);
void updateCameraInfo(FILE*); 