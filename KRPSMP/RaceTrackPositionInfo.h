#pragma once

HANDLE raceTrackPositionInfoFile;
RaceTrackPositionInfo_t* raceTrackPositionInfoView;

int initRaceTrackPositionInfo(FILE*);
void deinitRaceTrackPositionInfo(FILE*);
void updateRaceTrackPositionInfo(FILE*);