#pragma once

HANDLE raceEntriesInfoFile;
RaceEntriesInfo_t* raceEntriesInfoView;

int initRaceEntriesInfo(FILE*);
void deinitRaceEntriesInfo(FILE*);
void updateRaceEntriesInfo(FILE*);