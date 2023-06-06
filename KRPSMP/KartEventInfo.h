#pragma once

HANDLE kartEventInfoFile;
KartEventInfo_t* kartEventInfoView;

int initKartEventInfo(FILE*);
void deinitKartEventInfo(FILE*);
void updateKartEventInfo(FILE*);