#pragma once

HANDLE raceLapInfoFile;
RaceLapInfo_t* raceLapInfoView;

int initRaceLapInfo(FILE*);
void deinitRaceLapInfo(FILE*);
void updateRaceLapInfo(FILE*);