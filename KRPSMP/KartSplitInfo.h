#pragma once

HANDLE kartSplitInfoFile;
KartSplitInfo_t* kartSplitInfoView;

int initKartSplitInfo(FILE*);
void deinitKartSplitInfo(FILE*);
void updateKartSplitInfo(FILE*);