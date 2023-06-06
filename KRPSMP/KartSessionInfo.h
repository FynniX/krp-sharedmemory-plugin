#pragma once

HANDLE kartSessionInfoFile;
KartSessionInfo_t* kartSessionInfoView;

int initKartSessionInfo(FILE*);
void deinitKartSessionInfo(FILE*);
void updateKartSessionInfo(FILE*);