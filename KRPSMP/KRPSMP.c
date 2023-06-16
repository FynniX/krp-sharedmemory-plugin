#include <stdio.h>
#include <direct.h>
#include <windows.h>
#include "SharedFileIn.h"
#include "SharedFileOut.h"
#include "Configuration.h"
#include "PluginInfo.h"
#include "KartEventInfo.h"
#include "KartSessionInfo.h"
#include "KartLapInfo.h"
#include "KartSplitInfo.h"
#include "KartTelemetryInfo.h"
#include "TrackSegmentInfo.h"
#include "RaceEventInfo.h"
#include "RaceAddEntryInfo.h"
#include "RaceRemoveEntryInfo.h"
#include "RaceSessionInfo.h"
#include "RaceSessionStateInfo.h"
#include "RaceLapInfo.h"
#include "RaceSplitInfo.h"
#include "RaceSpeedInfo.h"
#include "RaceCommunicationInfo.h"
#include "RaceClassificationInfo.h"
#include "RaceTrackPositionInfo.h"
#include "RaceVehicleDataInfo.h"
#include "KRPSMP.h"

char* GetModID() {
	return "krp";
}

int GetModDataVersion() {
	return 6;
}

int GetInterfaceVersion() {
	return 9;
}

/* called when software is started */
int Startup(char* _szSavePath) {
	fopen_s(&logFile, "krpsmp_log.txt", "wt");
	if (!logFile) return -1;

	fprintf(logFile, "KRPSMP: Startup\n");
	fprintf(logFile, "%d\n", sizeof(RaceVehicleDataInfo_t));

	Configuration_t config = getConfiguration();
	if (config.enable == 0 || config.rate == -1) return -1;

	if (initPluginInfo(logFile) == -1) return -1;
	if (initKartEventInfo(logFile) == -1) return -1;
	if (initKartSessionInfo(logFile) == -1) return -1;
	if (initKartLapInfo(logFile) == -1) return -1;
	if (initKartSplitInfo(logFile) == -1) return -1;
	if (initKartTelemetryInfo(logFile) == -1) return -1;

	if (initTrackSegmentInfo(logFile) == -1) return -1;

	if (initRaceEventInfo(logFile) == -1) return -1;
	if (initRaceAddEntryInfo(logFile) == -1) return -1;
	if (initRaceRemoveEntryInfo(logFile) == -1) return -1;
	if (initRaceSessionInfo(logFile) == -1) return -1;
	if (initRaceSessionStateInfo(logFile) == -1) return -1;
	if (initRaceLapInfo(logFile) == -1) return -1;
	if (initRaceSplitInfo(logFile) == -1) return -1;
	if (initRaceSpeedInfo(logFile) == -1) return -1;
	if (initRaceCommunicationInfo(logFile) == -1) return -1;
	if (initRaceClassificationInfo(logFile) == -1) return -1;
	if (initRaceTrackPositionInfo(logFile) == -1) return -1;
	if (initRaceVehicleDataInfo(logFile) == -1) return -1;

	pluginInfoView->m_PluginRate = config.rate;
	pluginInfoView->m_PluginVersion = 1;
	updatePluginInfo(logFile);

	/*
	return value is requested rate
	0 = 100hz; 1 = 50hz; 2 = 20hz; 3 = 10hz; -1 = disable
	*/
	return config.rate;
}

/* called when software is closed */
void Shutdown() {
	fprintf(logFile, "KRPSMP: Shutdown\n");

	pluginInfoView->m_iState = -1;
	updatePluginInfo(logFile);

	deinitPluginInfo(logFile);
	deinitKartEventInfo(logFile);
	deinitKartSessionInfo(logFile);
	deinitKartLapInfo(logFile);
	deinitKartSplitInfo(logFile);
	deinitKartTelemetryInfo(logFile);

	deinitTrackSegmentInfo(logFile);

	deinitRaceEventInfo(logFile);
	deinitRaceAddEntryInfo(logFile);
	deinitRaceRemoveEntryInfo(logFile);
	deinitRaceSessionInfo(logFile);
	deinitRaceSessionStateInfo(logFile);
	deinitRaceLapInfo(logFile);
	deinitRaceSplitInfo(logFile);
	deinitRaceSpeedInfo(logFile);
	deinitRaceCommunicationInfo(logFile);
	deinitRaceClassificationInfo(logFile);
	deinitRaceTrackPositionInfo(logFile);
	deinitRaceVehicleDataInfo(logFile);

	if (logFile) fclose(logFile);
}

/* called when event is initialized. This function is optional */
void EventInit(void* _pData, int _iDataSize) {
	kartEventInfoView->m_KartEvent = *((SPluginsKartEvent_t*)_pData);
	updateKartEventInfo(logFile);
}

/* called when event is closed. This function is optional */
void EventDeinit() {
	kartEventInfoView->m_KartEvent = *((SPluginsKartEvent_t*) 0);
	updateKartEventInfo(logFile);
}

/* called when kart goes to track. This function is optional */
void RunInit(void* _pData, int _iDataSize) {
	kartSessionInfoView->m_KartSession = *((SPluginsKartSession_t*)_pData);
	updateKartSessionInfo(logFile);
}

/* called when kart leaves the track. This function is optional */
void RunDeinit() {
	pluginInfoView->m_iState = 0;
	updatePluginInfo(logFile);
	kartSessionInfoView->m_KartSession = *((SPluginsKartSession_t*)0);
	updateKartSessionInfo(logFile);
}

/* called when simulation is paused. This function is optional */
void RunStop() {
	pluginInfoView->m_iState = 1;
	updatePluginInfo(logFile);
}

/* called when a new lap is recorded. This function is optional */
void RunLap(void* _pData, int _iDataSize) {
	kartLapInfoView->m_KartLap = *((SPluginsKartLap_t*)_pData);
	updateKartLapInfo(logFile);
}

/* called when a split is crossed. This function is optional */
void RunSplit(void* _pData, int _iDataSize) {
	kartSplitInfoView->m_KartSplit = *((SPluginsKartSplit_t*)_pData);
	updateKartSplitInfo(logFile);
}

/* _fTime is the ontrack time, in seconds. _fPos is the position on centerline, from 0 to 1. This function is optional */
void RunTelemetry(void* _pData, int _iDataSize, float _fTime, float _fPos) {
	pluginInfoView->m_iState = 2;
	updatePluginInfo(logFile);

	kartTelemetryInfoView->_fTime = _fTime;
	kartTelemetryInfoView->_fPos = _fPos;
	kartTelemetryInfoView->m_KartData = *((SPluginsKartData_t*)_pData);
	updateKartTelemetryInfo(logFile);
}

/*
_pfRaceData is a pointer to a float array with the longitudinal position of the start / finish line, splits and speedtrap.
This function is optional
*/
void TrackCenterline(int _iNumSegments, SPluginsTrackSegment_t* _pasSegment, float* _pfRaceData) {
	trackSegmentInfoView->_iNumSegments = _iNumSegments;

	for (int i = 0; i < _iNumSegments; i++)
		trackSegmentInfoView->m_TrackSegments[i] = *(_pasSegment + i);

	for (int i = 0; i < 4; i++)
		trackSegmentInfoView->m_RaceData[i] = *(_pfRaceData + i);

	updateTrackSegmentInfo(logFile);
}

/* called when event is initialized or a replay is loaded. This function is optional */
void RaceEvent(void* _pData, int _iDataSize) {
	raceEventInfoView->m_RaceEvent = *((SPluginsRaceEvent_t*)_pData);
	updateRaceEventInfo(logFile);
}

/* called when event is closed. This function is optional */
void RaceDeinit() {
	raceEventInfoView->m_RaceEvent = *((SPluginsRaceEvent_t*)0);
	updateRaceEventInfo(logFile);
}

/* This function is optional */
void RaceAddEntry(void* _pData, int _iDataSize) {
	raceAddEntryInfoView->m_RaceAddEntry = *((SPluginsRaceAddEntry_t*)_pData);
	updateRaceAddEntryInfo(logFile);
}

/* This function is optional */
void RaceRemoveEntry(void* _pData, int _iDataSize) {
	raceRemoveEntryInfoView->m_RaceRemoveEntry = *((SPluginsRaceRemoveEntry_t*)_pData);
	updateRaceRemoveEntryInfo(logFile);
}

/* This function is optional */
void RaceSession(void* _pData, int _iDataSize) {
	raceSessionInfoView->m_RaceSession = *((SPluginsRaceSession_t*)_pData);
	updateRaceSessionInfo(logFile);
}

/* This function is optional */
void RaceSessionState(void* _pData, int _iDataSize) {
	raceSessionStateInfoView->m_RaceSessionState = *((SPluginsRaceSessionState_t*)_pData);
	updateRaceSessionStateInfo(logFile);
}

/* This function is optional */
void RaceLap(void* _pData, int _iDataSize) {
	raceLapInfoView->m_RaceLap = *((SPluginsRaceLap_t*)_pData);
	updateRaceLapInfo(logFile);
}

/* This function is optional */
void RaceSplit(void* _pData, int _iDataSize) {
	raceSplitInfoView->m_RaceSplit = *((SPluginsRaceSplit_t*)_pData);
	updateRaceSplitInfo(logFile);
}

/* This function is optional */
void RaceSpeed(void* _pData, int _iDataSize) {
	raceSpeedInfoView->m_RaceSpeed = *((SPluginsRaceSpeed_t*)_pData);
	updateRaceSpeedInfo(logFile);
}

/* This function is optional */
void RaceCommunication(void* _pData, int _iDataSize) {
	raceCommunicationInfoView->m_RaceCommunication = *((SPluginsRaceCommunication_t*)_pData);
	updateRaceCommunicationInfo(logFile);
}

/* The number of elements of _pArray if given by m_iNumEntries in _pData. This function is optional */
void RaceClassification(void* _pData, int _iDataSize, void* _pArray, int _iElemSize) {
	SPluginsRaceClassification_t* psRaceClassification;
	SPluginsRaceClassificationEntry_t* pasRaceClassificationEntry;

	psRaceClassification = (SPluginsRaceClassification_t*)_pData;
	pasRaceClassificationEntry = (SPluginsRaceClassificationEntry_t*)_pArray;

	raceClassificationInfoView->m_RaceClassification = *(psRaceClassification);
	for (int i = 0; i < psRaceClassification->m_iNumEntries; i++)
		raceClassificationInfoView->m_RaceEntries[i] = *(pasRaceClassificationEntry + i);

	updateRaceClassificationInfo(logFile);
}

/* This function is optional */
void RaceTrackPosition(int _iNumVehicles, void* _pArray, int _iElemSize) {
	SPluginsRaceTrackPosition_t* pasRaceTrackPosition;

	pasRaceTrackPosition = (SPluginsRaceTrackPosition_t*)_pArray;

	raceTrackPositionInfoView->_iNumVehicles = _iNumVehicles;
	for (int i = 0; i < _iNumVehicles; i++)
		raceTrackPositionInfoView->m_RaceTrackPositions[i] = *(pasRaceTrackPosition + i);

	updateRaceTrackPositionInfo(logFile);
}

/* This function is optional */
void RaceVehicleData(void* _pData, int _iDataSize) {
	raceVehicleDataInfoView->m_RaceVehicleData = *((SPluginsRaceVehicleData_t*)_pData);
	updateRaceVehicleDataInfo(logFile);
}