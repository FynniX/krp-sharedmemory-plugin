#ifndef SHAREDFILEOUT_H
#define SHAREDFILEOUT_H

#include "SharedFileIn.h"

// Enums

// 0: software running; 1: on-track, simulation paused; 2: on-track, simulation running
typedef enum
{
    MENU = 0,
    PAUSED = 1,
    RUNNING = 2,
    CLOSED = -1,
} GameState_t;

// 0: on track; 1: spectate; 2: replay
typedef enum
{
    ONTRACK = 0,
    SPECTATE = 1,
    REPLAY = 2,
} TrackState_t;

// 0 = 100hz; 1 = 50hz; 2 = 20hz; 3 = 10hz
typedef enum
{
    HUNDRED = 0,
    FIFTY = 1,
    TWENTY = 2,
    TEN = 3,
} ConfigurationRate_t;

// Buffer names

#define MB_PLUGININFO_FILE_NAME "Local\\KRPSMP__PluginInfo"

#define MB_KARTEVENTINFO_FILE_NAME "Local\\KRPSMP__KartEventInfo"
#define MB_KARTSESSIONINFO_FILE_NAME "Local\\KRPSMP__KartSessionInfo"
#define MB_KARTTELEMETRYINFO_FILE_NAME "Local\\KRPSMP__KartTelemetryInfo"
#define MB_KARTLAPSINFO_FILE_NAME "Local\\KRPSMP__KartLapsInfo"
#define MB_KARTSPLITSINFO_FILE_NAME "Local\\KRPSMP__KartSplitsInfo"

#define MB_TRACKSEGMENTSINFO_FILE_NAME "Local\\KRPSMP__TrackSegmentsInfo"

#define MB_RACEEVENTINFO_FILE_NAME "Local\\KRPSMP__RaceEventInfo"
#define MB_RACEENTRIESINFO_FILE_NAME "Local\\KRPSMP__RaceEntriesInfo"
#define MB_RACESESSIONINFO_FILE_NAME "Local\\KRPSMP__RaceSessionInfo"
#define MB_RACELAPSINFO_FILE_NAME "Local\\KRPSMP__RaceLapsInfo"
#define MB_RACESPLITSINFO_FILE_NAME "Local\\KRPSMP__RaceSplitsInfo"
#define MB_RACESPEEDSINFO_FILE_NAME "Local\\KRPSMP__RaceSpeedsInfo"
#define MB_RACECOMMUNICATIONSINFO_FILE_NAME "Local\\KRPSMP__RaceCommunicationsInfo"
#define MB_RACECLASSIFICATIONINFO_FILE_NAME "Local\\KRPSMP__RaceClassificationInfo"
#define MB_RACETRACKPOSITIONSINFO_FILE_NAME "Local\\KRPSMP__RaceTrackPositionsInfo"
#define MB_RACEVEHICLESDATAINFO_FILE_NAME "Local\\KRPSMP__RaceVehiclesDataInfo"

#define MB_CAMERASINFO_FILE_NAME "Local\\KRPSMP__CamerasInfo"
#define MB_CAMERASCONTROLINFO_FILE_NAME "Local\\KRPSMP__CamerasControlInfo"

#define TRACK_POINTS 4
#define LAPS_AMOUNT 10
#define SPLITS_AMOUNT 10
#define CAMERAS_AMOUNT 20
#define COMMUNICATIONS_AMOUNT 20
#define ENTRIES_AMOUNT 50
#define SEGMENTS_AMOUNT 200
#define STRING_LENGTH 100

// Buffer definitions

typedef struct
{
    // Message identifier
    int m_id;
} BaseInfo_t;

typedef struct
{
    // Message identifier
    int m_id;
    // 0: software running; 1: on-track, simulation paused; 2: on-track, simulation running; -1: software closed
    GameState_t m_GameState;
    // 0: on track; 1: spectate; 2: replay
    TrackState_t m_TrackState;
    // 0 = 100hz; 1 = 50hz; 2 = 20hz; 3 = 10hz
    ConfigurationRate_t m_PluginRate;
    // Plugin version
    int m_PluginVersion;
} PluginInfo_t;

/******************************************************************************
structures for simulated kart
******************************************************************************/

typedef struct 
{
    // Message identifier
    int m_id;
    SPluginsKartEvent_t m_KartEvent;
} KartEventInfo_t;

typedef struct 
{
    // Message identifier
    int m_id;
    SPluginsKartSession_t m_KartSession;
} KartSessionInfo_t;

typedef struct 
{
    // Message identifier
    int m_id;
    // _fTime is the ontrack time, in seconds.
    float _fTime;
    // _fPos is the position on centerline, from 0 to 1.
    float _fPos;
    SPluginsKartData_t m_KartData;
} KartTelemetryInfo_t;

typedef struct 
{
    // Message identifier
    int m_id;
    int _iNumLaps;
    SPluginsKartLap_t m_KartLaps[LAPS_AMOUNT];
} KartLapsInfo_t;

typedef struct 
{
    // Message identifier
    int m_id;
    int _iNumSplits;
    SPluginsKartSplit_t m_KartSplits[SPLITS_AMOUNT];
} KartSplitsInfo_t;

/******************************************************************************
structures for track center line
******************************************************************************/

typedef struct 
{
    // Message identifier
    int m_id;
    int _iNumSegments;
    SPluginsTrackSegment_t m_TrackSegments[SEGMENTS_AMOUNT];
    float m_RaceData[TRACK_POINTS];
} TrackSegmentsInfo_t;

/******************************************************************************
structures for race data
******************************************************************************/

typedef struct 
{
    // Message identifier
    int m_id;
    SPluginsRaceEvent_t m_RaceEvent;
} RaceEventInfo_t;

typedef struct 
{
    // Message identifier
    int m_id;
    int _iNumEntries;
    SPluginsRaceAddEntry_t m_RaceEntries[ENTRIES_AMOUNT];
} RaceEntriesInfo_t;

typedef struct 
{
    // Message identifier
    int m_id;
    SPluginsRaceSession_t m_RaceSession;
} RaceSessionInfo_t;

typedef struct 
{
    // Message identifier
    int m_id;
    int _iNumLaps;
    SPluginsRaceLap_t m_RaceLaps[LAPS_AMOUNT];
} RaceLapsInfo_t;

typedef struct 
{
    // Message identifier
    int m_id;
    int _iNumSplits;
    SPluginsRaceSplit_t m_RaceSplits[SPLITS_AMOUNT];
} RaceSplitsInfo_t;

typedef struct 
{
    // Message identifier
    int m_id;
    SPluginsRaceSpeed_t m_RaceSpeeds[ENTRIES_AMOUNT];
} RaceSpeedsInfo_t;

typedef struct 
{
    // Message identifier
    int m_id;
    int _iNumCommunications;
    SPluginsRaceCommunication_t m_RaceCommunications[COMMUNICATIONS_AMOUNT];
} RaceCommunicationsInfo_t;

typedef struct 
{
    // Message identifier
    int m_id;
    SPluginsRaceClassification_t m_RaceClassification;
    SPluginsRaceClassificationEntry_t m_RaceClassificationEntries[ENTRIES_AMOUNT];
} RaceClassificationInfo_t;

typedef struct 
{
    // Message identifier
    int m_id;
    int _iNumVehicles;
    SPluginsRaceTrackPosition_t m_RaceTrackPositions[ENTRIES_AMOUNT];
} RaceTrackPositionsInfo_t;

typedef struct 
{
    // Message identifier
    int m_id;
    SPluginsRaceVehicleData_t m_RaceVehiclesData[ENTRIES_AMOUNT];
} RaceVehiclesDataInfo_t;

/******************************************************************************
structures for control of replay
******************************************************************************/

typedef struct 
{
    // Message identifier
    int m_id;
    int currentVehicle;
    int currentCamera;
    int _iNumVehicles;
    int _iNumCameras;
    SPluginsSpectateVehicle_t m_Vehicles[ENTRIES_AMOUNT];
    char m_CameraNames[CAMERAS_AMOUNT][STRING_LENGTH];
} CamerasInfo_t;

typedef struct 
{
    // Message identifier
    int m_id;
    int selectedVehicle;
    int selectedCamera;
} CamerasControlInfo_t;

#endif