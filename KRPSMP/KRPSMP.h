#pragma once

#ifdef KRPSMP_EXPORTS
#define KRPSMP_API __declspec(dllexport)
#else
#define KRPSMP_API __declspec(dllimport)
#endif

FILE* logFile;

KRPSMP_API char* GetModID();
KRPSMP_API int GetModDataVersion();
KRPSMP_API int GetInterfaceVersion();

KRPSMP_API int Startup(char*);
KRPSMP_API void Shutdown();
KRPSMP_API void EventInit(void* _pData, int _iDataSize);
KRPSMP_API void EventDeinit();
KRPSMP_API void RunInit(void* _pData, int _iDataSize);
KRPSMP_API void RunDeinit();
KRPSMP_API void RunStop();
KRPSMP_API void RunLap(void* _pData, int _iDataSize);
KRPSMP_API void RunSplit(void* _pData, int _iDataSize);
KRPSMP_API void RunTelemetry(void* _pData, int _iDataSize, float _fTime, float _fPos);

KRPSMP_API int DrawInit(int* _piNumSprites, char** _pszSpriteName, int* _piNumFonts, char** _pszFontName);
KRPSMP_API void Draw(int _iState, int* _piNumQuads, void** _ppQuad, int* _piNumString, void** _ppString);

KRPSMP_API void TrackCenterline(int _iNumSegments, SPluginsTrackSegment_t* _pasSegment, float* _pfRaceData);

KRPSMP_API void RaceEvent(void* _pData, int _iDataSize);
KRPSMP_API void RaceDeinit();
KRPSMP_API void RaceAddEntry(void* _pData, int _iDataSize);
KRPSMP_API void RaceRemoveEntry(void* _pData, int _iDataSize);
KRPSMP_API void RaceSession(void* _pData, int _iDataSize);
KRPSMP_API void RaceSessionState(void* _pData, int _iDataSize);
KRPSMP_API void RaceLap(void* _pData, int _iDataSize);
KRPSMP_API void RaceSplit(void* _pData, int _iDataSize);
KRPSMP_API void RaceSpeed(void* _pData, int _iDataSize);
KRPSMP_API void RaceCommunication(void* _pData, int _iDataSize);
KRPSMP_API void RaceClassification(void* _pData, int _iDataSize, void* _pArray, int _iElemSize);
KRPSMP_API void RaceTrackPosition(int _iNumVehicles, void* _pArray, int _iElemSize);
KRPSMP_API void RaceVehicleData(void* _pData, int _iDataSize);

KRPSMP_API int SpectateVehicles(int _iNumVehicles, void* _pVehicleData, int _iCurSelection, int* _piSelect);
KRPSMP_API int SpectateCameras(int _iNumCameras, void* _pCameraData, int _iCurSelection, int* _piSelect);