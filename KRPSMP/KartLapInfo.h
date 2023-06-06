#pragma once

HANDLE kartLapInfoFile;
KartLapInfo_t* kartLapInfoView;

int initKartLapInfo(FILE*);
void deinitKartLapInfo(FILE*);
void updateKartLapInfo(FILE*);