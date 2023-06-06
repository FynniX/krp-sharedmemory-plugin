#pragma once

HANDLE raceClassificationInfoFile;
RaceClassificationInfo_t* raceClassificationInfoView;

int initRaceClassificationInfo(FILE*);
void deinitRaceClassificationInfo(FILE*);
void updateRaceClassificationInfo(FILE*);