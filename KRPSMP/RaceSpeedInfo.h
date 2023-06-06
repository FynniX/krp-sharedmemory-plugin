#pragma once

HANDLE raceSpeedInfoFile;
RaceSpeedInfo_t* raceSpeedInfoView;

int initRaceSpeedInfo(FILE*);
void deinitRaceSpeedInfo(FILE*);
void updateRaceSpeedInfo(FILE*);