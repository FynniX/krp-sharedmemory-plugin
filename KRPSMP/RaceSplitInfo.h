#pragma once

HANDLE raceSplitInfoFile;
RaceSplitInfo_t* raceSplitInfoView;

int initRaceSplitInfo(FILE*);
void deinitRaceSplitInfo(FILE*);
void updateRaceSplitInfo(FILE*);