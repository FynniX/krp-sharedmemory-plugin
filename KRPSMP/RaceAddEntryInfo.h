#pragma once

HANDLE raceAddEntryInfoFile;
RaceAddEntryInfo_t* raceAddEntryInfoView;

int initRaceAddEntryInfo(FILE*);
void deinitRaceAddEntryInfo(FILE*);
void updateRaceAddEntryInfo(FILE*);