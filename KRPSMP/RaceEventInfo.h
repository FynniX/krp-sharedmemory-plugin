#pragma once

HANDLE raceEventInfoFile;
RaceEventInfo_t* raceEventInfoView;

int initRaceEventInfo(FILE*);
void deinitRaceEventInfo(FILE*);
void updateRaceEventInfo(FILE*);