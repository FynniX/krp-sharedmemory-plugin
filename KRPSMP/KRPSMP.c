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
#include "RaceEntriesInfo.h"
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
#include "CameraInfo.h"
#include "CameraSet.h"
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
	Configuration_t config = getConfiguration();
	if (config.enable == 0 || config.rate == -1) return -1;

	if(config.logging == 1)
		fopen_s(&logFile, "krpsmp_log.txt", "wt");
	if (!logFile && config.logging == 1) return -1;

	if (logFile)
		fprintf(logFile, "KRPSMP: Startup\n");

	if (initPluginInfo(logFile) == -1) return -1;
	if (initKartEventInfo(logFile) == -1) return -1;
	if (initKartSessionInfo(logFile) == -1) return -1;
	if (initKartLapInfo(logFile) == -1) return -1;
	if (initKartSplitInfo(logFile) == -1) return -1;
	if (initKartTelemetryInfo(logFile) == -1) return -1;

	if (initTrackSegmentInfo(logFile) == -1) return -1;

	if (initRaceEventInfo(logFile) == -1) return -1;
	if (initRaceEntriesInfo(logFile) == -1) return -1;
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
	if (initCameraInfo(logFile) == -1) return -1;

	pluginInfoView->m_PluginRate = config.rate;
	pluginInfoView->m_PluginVersion = 11;
	updatePluginInfo(logFile);

	/*
	return value is requested rate
	0 = 100hz; 1 = 50hz; 2 = 20hz; 3 = 10hz; -1 = disable
	*/
	return config.rate;
}

/* called when software is closed */
void Shutdown() {
	if (logFile)
		fprintf(logFile, "KRPSMP: Shutdown\n");

	SPluginsRaceAddEntry_t emptyEntry = { 0 };
	for (int i = 0; i < 100; i++)
		raceEntriesInfoView->m_RaceEntries[i] = emptyEntry;
	raceEntriesInfoView->_iNumEntries = 0;
	updateRaceEntriesInfo(logFile);

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
	deinitRaceEntriesInfo(logFile);
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
	deinitCameraInfo(logFile);

	disconnectCameraSet(logFile);

	if (logFile) fclose(logFile);
}

/* called when event is initialized. This function is optional */
void EventInit(void* _pData, int _iDataSize) {
	kartEventInfoView->m_KartEvent = *((SPluginsKartEvent_t*)_pData);
	updateKartEventInfo(logFile);
}

/* called when event is closed. This function is optional */
void EventDeinit() {
	SPluginsKartEvent_t data = { 0 };
	kartEventInfoView->m_KartEvent = data;
	updateKartEventInfo(logFile);

	SPluginsRaceAddEntry_t emptyEntry = { 0 };
	for (int i = 0; i < 100; i++)
		raceEntriesInfoView->m_RaceEntries[i] = emptyEntry;
	raceEntriesInfoView->_iNumEntries = 0;
	updateRaceEntriesInfo(logFile);
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

	SPluginsKartSession_t data = { 0 };
	kartSessionInfoView->m_KartSession = data;
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
called when software is started.
Set _piNumSprites to the number of zero-separated filenames in _pszSpriteName.
Set _piNumFonts to the number of zero-separated filenames in _pszFontName.
The base path for the sprite and font files is the plugins folder.
This function is optional
*/
int DrawInit(int* _piNumSprites, char** _pszSpriteName, int* _piNumFonts, char** _pszFontName)
{
	/*
	return 0 if pointers are set
	*/
	return -1;
}

/*
_iState: 0 = on track; 1 = spectate; 2 = replay.
Set _piNumQuads to the number of quads to draw.
Set _ppQuad to an array of SPluginQuad_t structures.
Set _piNumString to the number of strings to draw.
Set _ppString to an array of SPluginString_t structures.
This function is optional
*/
void Draw(int _iState, int* _piNumQuads, void** _ppQuad, int* _piNumString, void** _ppString) {
	*_piNumQuads = 0;
	*_piNumString = 0;
	pluginInfoView->m_iState2 = _iState;
	updatePluginInfo(logFile);
}

/*
_pfRaceData is a pointer to a float array with the longitudinal position of the start / finish line, splits and speedtrap.
This function is optional
*/
void TrackCenterline(int _iNumSegments, SPluginsTrackSegment_t* _pasSegment, float* _pfRaceData) {
	trackSegmentInfoView->_iNumSegments = _iNumSegments;

	for (int i = 0; i < 200; i++) {
		if (i < _iNumSegments) {
			trackSegmentInfoView->m_TrackSegments[i] = *(_pasSegment + i);
		} else {
			SPluginsTrackSegment_t data = { 0 };
			trackSegmentInfoView->m_TrackSegments[i] = data;
		}
	}
		
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
	SPluginsRaceEvent_t data = { 0 };
	raceEventInfoView->m_RaceEvent = data;
	updateRaceEventInfo(logFile);
}

/* This function is optional */
void RaceAddEntry(void* _pData, int _iDataSize) {
	raceAddEntryInfoView->m_RaceAddEntry = *((SPluginsRaceAddEntry_t*)_pData);

	int nextIndex = raceEntriesInfoView->_iNumEntries;
	int newRaceNum = raceAddEntryInfoView->m_RaceAddEntry.m_iRaceNum;
	for (int i = 0; i < 100; i++) {
		int currentRaceNum = raceEntriesInfoView->m_RaceEntries[i].m_iRaceNum;
		if (newRaceNum != currentRaceNum) continue;
		nextIndex = i;
		break;
	}

	raceEntriesInfoView->m_RaceEntries[nextIndex] = raceAddEntryInfoView->m_RaceAddEntry;
	if (nextIndex == raceEntriesInfoView->_iNumEntries) raceEntriesInfoView->_iNumEntries++;

	updateRaceAddEntryInfo(logFile);
	updateRaceEntriesInfo(logFile);
}

/* This function is optional */
void RaceRemoveEntry(void* _pData, int _iDataSize) {
	raceRemoveEntryInfoView->m_RaceRemoveEntry = *((SPluginsRaceRemoveEntry_t*)_pData);

	int index = -1;
	int removeRaceNum = raceRemoveEntryInfoView->m_RaceRemoveEntry.m_iRaceNum;
	for (int i = 0; i < 100; i++) {
		int currentRaceNum = raceEntriesInfoView->m_RaceEntries[i].m_iRaceNum;
		if (removeRaceNum != currentRaceNum) continue;
		index = i;
		break;
	}

	if (index != -1) {
		for (int i = index; i < 99; i++)
			raceEntriesInfoView->m_RaceEntries[i] = raceEntriesInfoView->m_RaceEntries[i + 1];
		SPluginsRaceAddEntry_t emptyEntry = { 0 };
		raceEntriesInfoView->m_RaceEntries[99] = emptyEntry;
		raceEntriesInfoView->_iNumEntries--;
	}

	updateRaceRemoveEntryInfo(logFile);
	updateRaceEntriesInfo(logFile);
}

/* This function is optional */
void RaceSession(void* _pData, int _iDataSize) {
	raceSessionInfoView->m_RaceSession = *((SPluginsRaceSession_t*)_pData);
	updateRaceSessionInfo(logFile);
}

/* This function is optional */
void RaceSessionState(void* _pData, int _iDataSize) {
	raceSessionStateInfoView->m_RaceSessionState = *((SPluginsRaceSessionState_t*)_pData);
	raceSessionInfoView->m_RaceSession.m_iSession = raceSessionStateInfoView->m_RaceSessionState.m_iSession;
	raceSessionInfoView->m_RaceSession.m_iSessionSeries = raceSessionStateInfoView->m_RaceSessionState.m_iSessionSeries;
	raceSessionInfoView->m_RaceSession.m_iSessionState = raceSessionStateInfoView->m_RaceSessionState.m_iSessionState;
	raceSessionInfoView->m_RaceSession.m_iSessionLength = raceSessionStateInfoView->m_RaceSessionState.m_iSessionLength;
	updateRaceSessionStateInfo(logFile);
	updateRaceSessionInfo(logFile);
}

/* This function is optional */
void RaceLap(void* _pData, int _iDataSize) {
	SPluginsRaceLap_t data = *((SPluginsRaceLap_t*)_pData);

	int index = -1;
	int raceNum = data.m_iRaceNum;
	for (int i = 0; i < 100; i++) {
		int currentRaceNum = raceEntriesInfoView->m_RaceEntries[i].m_iRaceNum;
		if (raceNum != currentRaceNum) continue;
		index = i;
		break;
	}

	raceLapInfoView->m_RaceLaps[0] = data;
	raceLapInfoView->m_RaceLaps[index + 1] = data;
	updateRaceLapInfo(logFile);
}

/* This function is optional */
void RaceSplit(void* _pData, int _iDataSize) {
	SPluginsRaceSplit_t data = *((SPluginsRaceSplit_t*)_pData);

	int index = -1;
	int raceNum = data.m_iRaceNum;
	for (int i = 0; i < 100; i++) {
		int currentRaceNum = raceEntriesInfoView->m_RaceEntries[i].m_iRaceNum;
		if (raceNum != currentRaceNum) continue;
		index = i;
		break;
	}

	raceSplitInfoView->m_RaceSplits[0] = data;
	raceSplitInfoView->m_RaceSplits[index + 1] = data;
	updateRaceSplitInfo(logFile);
}

/* This function is optional */
void RaceSpeed(void* _pData, int _iDataSize) {
	SPluginsRaceSpeed_t data = *((SPluginsRaceSpeed_t*)_pData);

	int index = -1;
	int raceNum = data.m_iRaceNum;
	for (int i = 0; i < 100; i++) {
		int currentRaceNum = raceEntriesInfoView->m_RaceEntries[i].m_iRaceNum;
		if (raceNum != currentRaceNum) continue;
		index = i;
		break;
	}

	raceSpeedInfoView->m_RaceSpeeds[0] = data;
	raceSpeedInfoView->m_RaceSpeeds[index + 1] = data;
	updateRaceSpeedInfo(logFile);
}

/* This function is optional */
void RaceCommunication(void* _pData, int _iDataSize) {
	SPluginsRaceCommunication_t data = *((SPluginsRaceCommunication_t*)_pData);

	int index = -1;
	int raceNum = data.m_iRaceNum;
	for (int i = 0; i < 100; i++) {
		int currentRaceNum = raceEntriesInfoView->m_RaceEntries[i].m_iRaceNum;
		if (raceNum != currentRaceNum) continue;
		index = i;
		break;
	}

	raceCommunicationInfoView->m_RaceCommunications[0] = data;
	raceCommunicationInfoView->m_RaceCommunications[index + 1] = data;
	updateRaceCommunicationInfo(logFile);
}

/* The number of elements of _pArray if given by m_iNumEntries in _pData. This function is optional */
void RaceClassification(void* _pData, int _iDataSize, void* _pArray, int _iElemSize) {
	SPluginsRaceClassification_t* psRaceClassification;
	SPluginsRaceClassificationEntry_t* pasRaceClassificationEntry;

	psRaceClassification = (SPluginsRaceClassification_t*)_pData;
	pasRaceClassificationEntry = (SPluginsRaceClassificationEntry_t*)_pArray;

	raceClassificationInfoView->m_RaceClassification = *(psRaceClassification);
	for (int i = 0; i < 100; i++) {
		if (i < psRaceClassification->m_iNumEntries) {
			int oldGap = raceClassificationInfoView->m_RaceEntries[i].m_iGap;
			int oldGapLaps = raceClassificationInfoView->m_RaceEntries[i].m_iGapLaps;
			SPluginsRaceClassificationEntry_t entry = *(pasRaceClassificationEntry + i);
			

			if ((oldGap > 0 && entry.m_iGap == 0) || (oldGapLaps > 0 && entry.m_iGapLaps == 0)) {
				entry.m_iGap = oldGap;
				entry.m_iGapLaps = oldGapLaps;
			}

			raceClassificationInfoView->m_RaceEntries[i] = entry;
		}
		else {
			SPluginsRaceClassificationEntry_t data = { 0 };
			raceClassificationInfoView->m_RaceEntries[i] = data;
		}
	}

	updateRaceClassificationInfo(logFile);
}

/* This function is optional */
void RaceTrackPosition(int _iNumVehicles, void* _pArray, int _iElemSize) {
	SPluginsRaceTrackPosition_t* pasRaceTrackPosition;
	pasRaceTrackPosition = (SPluginsRaceTrackPosition_t*)_pArray;

	raceTrackPositionInfoView->_iNumVehicles = _iNumVehicles;
	for (int i = 0; i < 100; i++) {
		if (i < _iNumVehicles) {
			raceTrackPositionInfoView->m_RaceTrackPositions[i] = *(pasRaceTrackPosition + i);
		}
		else {
			SPluginsRaceTrackPosition_t data = { 0 };
			raceTrackPositionInfoView->m_RaceTrackPositions[i] = data;
		}
	}
		

	updateRaceTrackPositionInfo(logFile);
}

/* This function is optional */
void RaceVehicleData(void* _pData, int _iDataSize) {
	SPluginsRaceVehicleData_t data = *((SPluginsRaceVehicleData_t*)_pData);

	int index = -1;
	int raceNum = data.m_iRaceNum;
	for (int i = 0; i < 100; i++) {
		int currentRaceNum = raceEntriesInfoView->m_RaceEntries[i].m_iRaceNum;
		if (raceNum != currentRaceNum) continue;
		index = i;
		break;
	}

	raceVehicleDataInfoView->m_RaceVehicleDatas[0] = data;
	raceVehicleDataInfoView->m_RaceVehicleDatas[index + 1] = data;
	updateRaceVehicleDataInfo(logFile);
}

/* Return 1 if _piSelect is set, from 0 to _iNumVehicles - 1 */
int SpectateVehicles(int _iNumVehicles, void* _pVehicleData, int _iCurSelection, int* _piSelect)
{
	CameraVehiclesInfo_t vehiclesInfo;
	SPluginsSpectateVehicle_t* pasVehicleData;
	pasVehicleData = (SPluginsSpectateVehicle_t*)_pVehicleData;

	int select = 0;
	CameraSet_t* cameraSetData = getCameraSet(logFile);
	if (cameraSetData != NULL && cameraSetData->isControlled == 1) {
		if (cameraSetData->selectedVehicle != _iCurSelection && cameraSetData->selectedVehicle >= 0 && cameraSetData->selectedVehicle < _iNumVehicles) {
			select = 1;
			*_piSelect = cameraSetData->selectedVehicle;
		} else {
			select = 0;
		}
	}

	vehiclesInfo._iNumVehicles = _iNumVehicles;
	vehiclesInfo._iCurSelection = _iCurSelection;

	for (int i = 0; i < 100; i++) {
		if (i < _iNumVehicles) {
			vehiclesInfo.m_VehicleData[i] = *(pasVehicleData + i);
		}
		else {
			SPluginsSpectateVehicle_t data = { 0 };
			vehiclesInfo.m_VehicleData[i] = data;
		}
	}

	cameraInfoView->m_VehiclesInfo = vehiclesInfo;
	updateCameraInfo(logFile);

	return select;
}

/* Return 1 if _piSelect is set, from 0 to _iNumCameras - 1 */
int SpectateCameras(int _iNumCameras, void* _pCameraData, int _iCurSelection, int* _piSelect)
{
	return 0;
	CameraCamerasInfo_t camerasInfo;
	char* pszCameraName = (char*)_pCameraData;

	int select = 0;
	CameraSet_t* cameraSetData = getCameraSet(logFile);
	if (cameraSetData != NULL && cameraSetData->isControlled == 1) {
		if (cameraSetData->selectedCamera != _iCurSelection && cameraSetData->selectedCamera >= 0 && cameraSetData->selectedCamera < _iNumCameras) {
			select = 1;
			*_piSelect = cameraSetData->selectedVehicle;
		}
		else {
			select = 0;
		}
	}

	camerasInfo._iNumCameras = _iNumCameras;
	camerasInfo._iCurSelection = _iCurSelection;

	for (int i = 0; i < 50; i++) {
		if (i < _iNumCameras) {
			size_t length = strlen(pszCameraName) + 1;
			pszCameraName += length;
			strcpy_s(camerasInfo.m_CameraNames[i], length, pszCameraName);
		} else {
			strcpy_s(camerasInfo.m_CameraNames[i], strlen("") + 1, "");
		}
	}

	cameraInfoView->m_CamerasInfo = camerasInfo;
	updateCameraInfo(logFile);

	return select;
}