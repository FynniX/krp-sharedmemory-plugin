#pragma once

// Plugin Info

typedef struct {
	//message id
	int m_id;
	//0: software running; 1: on-track, simulation paused; 2: on-track, simulation running
	int m_iState;
	//0 = 100hz; 1 = 50hz; 2 = 20hz; 3 = 10hz; -1 = disable
	int m_PluginRate;
	int m_PluginVersion;
} PluginInfo_t;

// Kart Events

typedef struct {
	int m_id;    													/* message id */
	SPluginsKartEvent_t m_KartEvent;
} KartEventInfo_t;

typedef struct {
	int m_id;    													/* message id */
	SPluginsKartSession_t m_KartSession;
} KartSessionInfo_t;

typedef struct {
	int m_id;    													/* message id */
	SPluginsKartLap_t m_KartLap;
} KartLapInfo_t;

typedef struct {
	int m_id;    													/* message id */
	SPluginsKartSplit_t m_KartSplit;
} KartSplitInfo_t;

typedef struct {
	int m_id;    													/* message id */
	float _fTime;
	float _fPos;
	SPluginsKartData_t m_KartData;
} KartTelemetryInfo_t;

//Track Centerline

typedef struct {
	int m_id;    													/* message id */
	int _iNumSegments;
	SPluginsTrackSegment_t m_TrackSegments[200];
	float m_RaceData[4];
} TrackSegmentInfo_t;

// Race Events

typedef struct {
	int m_id;    													/* message id */
	SPluginsRaceEvent_t m_RaceEvent;
} RaceEventInfo_t;

typedef struct {
	int m_id;    													/* message id */
	int _iNumEntries;
	SPluginsRaceAddEntry_t m_RaceEntries[100];
} RaceEntriesInfo_t;

typedef struct {
	int m_id;    													/* message id */
	SPluginsRaceAddEntry_t m_RaceAddEntry;
} RaceAddEntryInfo_t;

typedef struct {
	int m_id;    													/* message id */
	SPluginsRaceRemoveEntry_t m_RaceRemoveEntry;
} RaceRemoveEntryInfo_t;

typedef struct {
	int m_id;    													/* message id */
	SPluginsRaceSession_t m_RaceSession;
} RaceSessionInfo_t;

typedef struct {
	int m_id;    													/* message id */
	SPluginsRaceSessionState_t m_RaceSessionState;
} RaceSessionStateInfo_t;

typedef struct {
	int m_id;    													/* message id */
	SPluginsRaceLap_t m_RaceLap;
} RaceLapInfo_t;

typedef struct {
	int m_id;    													/* message id */
	SPluginsRaceSplit_t m_RaceSplit;
} RaceSplitInfo_t;

typedef struct {
	int m_id;    													/* message id */
	SPluginsRaceSpeed_t m_RaceSpeed;
} RaceSpeedInfo_t;

typedef struct {
	int m_id;    													/* message id */
	SPluginsRaceCommunication_t m_RaceCommunication;
} RaceCommunicationInfo_t;

typedef struct
{
	int m_iRaceNum;										/* race number */
	int m_iState;										/* 1 = DNS; 2 = retired; 3 = DSQ */
	int m_iBestLap;										/* milliseconds */
	SPluginsRaceLap_t m_LastLap;
	SPluginsRaceSplit_t m_LastSplit;
	SPluginsRaceSpeed_t m_LastSpeed;
	SPluginsRaceCommunication_t m_LastCommunication;
	SPluginsRaceVehicleData_t m_LastVehicleData;
	float m_fBestSpeed;									/* meters/second */
	int m_iBestLapNum;									/* best lap index */
	int m_iNumLaps;										/* number of laps */
	int m_iGap;											/* milliseconds */
	int m_iGapLaps;
	int m_iPenalty;										/* milliseconds */
	int m_iPit;											/* 0 = on track; 1 = in the pits */
} SPluginsRaceClassificationEntry2_t;

typedef struct {
	int m_id;    													/* message id */
	SPluginsRaceClassification_t m_RaceClassification;
	SPluginsRaceClassificationEntry2_t m_RaceEntries[100];
} RaceClassificationInfo_t;

typedef struct {
	int m_id;    													/* message id */
	int _iNumVehicles;
	SPluginsRaceTrackPosition_t m_RaceTrackPositions[100];
} RaceTrackPositionInfo_t;

typedef struct {
	int m_id;    													/* message id */
	SPluginsRaceVehicleData_t m_RaceVehicleData;
} RaceVehicleDataInfo_t;