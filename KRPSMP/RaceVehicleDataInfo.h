#pragma once

HANDLE raceVehicleDataInfoFile;
RaceVehicleDataInfo_t* raceVehicleDataInfoView;

int initRaceVehicleDataInfo(FILE*);
void deinitRaceVehicleDataInfo(FILE*);
void updateRaceVehicleDataInfo(FILE*);