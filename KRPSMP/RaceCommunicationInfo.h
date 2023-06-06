#pragma once

HANDLE raceCommunicationInfoFile;
RaceCommunicationInfo_t* raceCommunicationInfoView;

int initRaceCommunicationInfo(FILE*);
void deinitRaceCommunicationInfo(FILE*);
void updateRaceCommunicationInfo(FILE*);