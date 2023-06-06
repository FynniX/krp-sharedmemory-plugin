#pragma once

HANDLE raceRemoveEntryInfoFile;
RaceRemoveEntryInfo_t* raceRemoveEntryInfoView;

int initRaceRemoveEntryInfo(FILE*);
void deinitRaceRemoveEntryInfo(FILE*);
void updateRaceRemoveEntryInfo(FILE*);