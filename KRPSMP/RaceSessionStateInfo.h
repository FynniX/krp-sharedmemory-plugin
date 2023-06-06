#pragma once

HANDLE raceSessionStateInfoFile;
RaceSessionStateInfo_t* raceSessionStateInfoView;

int initRaceSessionStateInfo(FILE*);
void deinitRaceSessionStateInfo(FILE*);
void updateRaceSessionStateInfo(FILE*);