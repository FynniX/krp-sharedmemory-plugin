#pragma once

HANDLE raceSessionInfoFile;
RaceSessionInfo_t* raceSessionInfoView;

int initRaceSessionInfo(FILE*);
void deinitRaceSessionInfo(FILE*);
void updateRaceSessionInfo(FILE*);