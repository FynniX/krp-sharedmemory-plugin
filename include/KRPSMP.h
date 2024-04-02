#ifndef KRPSMP_H
#define KRPSMP_H

#include <iostream>
#include "Logger/Logger.h"
#include "Configuration/Configuration.h"
#include "MappedBuffer/MappedBuffer.h"
#include "SharedFileIn.h"
#include "SharedFileOut.h"

#define KRPSMP_API __declspec(dllexport)

#define KRPSMP_VERSION 14

Configuration *configuration;
Logger *logger;

// Mappings
MappedBuffer<PluginInfo_t> *PluginInfo;

MappedBuffer<KartEventInfo_t> *KartEventInfo;
MappedBuffer<KartSessionInfo_t> *KartSessionInfo;
MappedBuffer<KartTelemetryInfo_t> *KartTelemetryInfo;
MappedBuffer<KartLapsInfo_t> *KartLapsInfo;
MappedBuffer<KartSplitsInfo_t> *KartSplitsInfo;

MappedBuffer<TrackSegmentsInfo_t> *TrackSegmentsInfo;

MappedBuffer<RaceEventInfo_t> *RaceEventInfo;
MappedBuffer<RaceEntriesInfo_t> *RaceEntriesInfo;
MappedBuffer<RaceSessionInfo_t> *RaceSessionInfo;
MappedBuffer<RaceLapsInfo_t> *RaceLapsInfo;
MappedBuffer<RaceSplitsInfo_t> *RaceSplitsInfo;
MappedBuffer<RaceSpeedsInfo_t> *RaceSpeedsInfo;
MappedBuffer<RaceCommunicationsInfo_t> *RaceCommunicationsInfo;
MappedBuffer<RaceClassificationInfo_t> *RaceClassificationInfo;
MappedBuffer<RaceTrackPositionsInfo_t> *RaceTrackPositionsInfo;
MappedBuffer<RaceVehiclesDataInfo_t> *RaceVehiclesDataInfo;

MappedBuffer<CamerasInfo_t> *CamerasInfo;
MappedBuffer<CamerasControlInfo_t> *CamerasControlInfo;

// Buffers
PluginInfo_t *PluginInfoView;

KartEventInfo_t *KartEventInfoView;
KartSessionInfo_t *KartSessionInfoView;
KartTelemetryInfo_t *KartTelemetryInfoView;
KartLapsInfo_t *KartLapsInfoView;
KartSplitsInfo_t *KartSplitsInfoView;

TrackSegmentsInfo_t *TrackSegmentsInfoView;

RaceEventInfo_t *RaceEventInfoView;
RaceEntriesInfo_t *RaceEntriesInfoView;
RaceSessionInfo_t *RaceSessionInfoView;
RaceLapsInfo_t *RaceLapsInfoView;
RaceSplitsInfo_t *RaceSplitsInfoView;
RaceSpeedsInfo_t *RaceSpeedsInfoView;
RaceCommunicationsInfo_t *RaceCommunicationsInfoView;
RaceClassificationInfo_t *RaceClassificationInfoView;
RaceTrackPositionsInfo_t *RaceTrackPositionsInfoView;
RaceVehiclesDataInfo_t *RaceVehiclesDataInfoView;

CamerasInfo_t *CamerasInfoView;
CamerasControlInfo_t *CamerasControlInfoView;

extern "C" KRPSMP_API char *GetModID();
extern "C" KRPSMP_API int GetModDataVersion();
extern "C" KRPSMP_API int GetInterfaceVersion();

extern "C" KRPSMP_API int Startup(char *_szSavePath);
extern "C" KRPSMP_API void Shutdown();

extern "C" KRPSMP_API void EventInit(void *_pData, int _iDataSize);
extern "C" KRPSMP_API void EventDeinit();
extern "C" KRPSMP_API void RunInit(void *_pData, int _iDataSize);
extern "C" KRPSMP_API void RunDeinit();
extern "C" KRPSMP_API void RunStart();
extern "C" KRPSMP_API void RunStop();
extern "C" KRPSMP_API void RunLap(void *_pData, int _iDataSize);
extern "C" KRPSMP_API void RunSplit(void *_pData, int _iDataSize);
extern "C" KRPSMP_API void RunTelemetry(void *_pData, int _iDataSize, float _fTime, float _fPos);

extern "C" KRPSMP_API int DrawInit(int *_piNumSprites, char **_pszSpriteName, int *_piNumFonts, char **_pszFontName);
extern "C" KRPSMP_API void Draw(int _iState, int *_piNumQuads, void **_ppQuad, int *_piNumString, void **_ppString);

extern "C" KRPSMP_API void TrackCenterline(int _iNumSegments,SPluginsTrackSegment_t *_pasSegment,float *_pfRaceData);

extern "C" KRPSMP_API void RaceEvent(void *_pData,int _iDataSize);
extern "C" KRPSMP_API void RaceDeinit();
extern "C" KRPSMP_API void RaceAddEntry(void *_pData,int _iDataSize);
extern "C" KRPSMP_API void RaceRemoveEntry(void *_pData,int _iDataSize);
extern "C" KRPSMP_API void RaceSession(void *_pData,int _iDataSize);
extern "C" KRPSMP_API void RaceSessionState(void *_pData,int _iDataSize);
extern "C" KRPSMP_API void RaceLap(void *_pData,int _iDataSize);
extern "C" KRPSMP_API void RaceSplit(void *_pData,int _iDataSize);
extern "C" KRPSMP_API void RaceSpeed(void *_pData,int _iDataSize);
extern "C" KRPSMP_API void RaceCommunication(void *_pData,int _iDataSize);
extern "C" KRPSMP_API void RaceClassification(void *_pData,int _iDataSize,void *_pArray,int _iElemSize);
extern "C" KRPSMP_API void RaceTrackPosition(int _iNumVehicles,void *_pArray,int _iElemSize);
extern "C" KRPSMP_API void RaceVehicleData(void *_pData,int _iDataSize);

extern "C" KRPSMP_API int SpectateVehicles(int _iNumVehicles,void *_pVehicleData,int _iCurSelection,int *_piSelect);
extern "C" KRPSMP_API int SpectateCameras(int _iNumCameras,void *_pCameraData,int _iCurSelection,int *_piSelect);

#endif