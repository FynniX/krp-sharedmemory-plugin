#pragma once

HANDLE kartTelemetryInfoFile;
KartTelemetryInfo_t* kartTelemetryInfoView;

int initKartTelemetryInfo(FILE*);
void deinitKartTelemetryInfo(FILE*);
void updateKartTelemetryInfo(FILE*);