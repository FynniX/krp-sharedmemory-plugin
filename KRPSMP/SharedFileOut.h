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
	SPluginsKartEvent_t* m_KartEvent;
} KartEventInfo_t;

typedef struct {
	int m_id;    													/* message id */
	SPluginsKartSession_t* m_KartSession;
} KartSessionInfo_t;

typedef struct {
	int m_id;    													/* message id */
	SPluginsKartLap_t* m_KartLap;
} KartLapInfo_t;

typedef struct {
	int m_id;    													/* message id */
	SPluginsKartSplit_t* m_KartSplit;
} KartSplitInfo_t;

typedef struct {
	int m_id;    													/* message id */
	float _fTime;
	float _fPos;
	SPluginsKartData_t* m_KartData;
} KartTelemetryInfo_t;

// Race Events

typedef struct {
	int m_id;    													/* message id */
	SPluginsRaceEvent_t* m_RaceEvent;
} RaceEventInfo_t;

typedef struct {
	int m_id;    													/* message id */
	SPluginsRaceAddEntry_t* m_RaceAddEntry;
} RaceAddEntryInfo_t;

typedef struct {
	int m_id;    													/* message id */
	SPluginsRaceRemoveEntry_t* m_RaceRemoveEntry;
} RaceRemoveEntryInfo_t;

typedef struct {
	int m_id;    													/* message id */
	SPluginsRaceSession_t* m_RaceSession;
} RaceSessionInfo_t;

typedef struct {
	int m_id;    													/* message id */
	SPluginsRaceSessionState_t* m_RaceSessionState;
} RaceSessionStateInfo_t;

typedef struct {
	int m_id;    													/* message id */
	SPluginsRaceLap_t* m_RaceLap;
} RaceLapInfo_t;

typedef struct {
	int m_id;    													/* message id */
	SPluginsRaceSplit_t* m_RaceSplit;
} RaceSplitInfo_t;

typedef struct {
	int m_id;    													/* message id */
	SPluginsRaceSpeed_t* m_RaceSpeed;
} RaceSpeedInfo_t;

typedef struct {
	int m_id;    													/* message id */
	SPluginsRaceCommunication_t* m_RaceCommunication;
} RaceCommunicationInfo_t;

typedef struct {
	int m_id;    													/* message id */
} RaceClassificationInfo_t;

typedef struct {
	int m_id;    													/* message id */
} RaceTrackPositionInfo_t;

typedef struct {
	int m_id;    													/* message id */
	SPluginsRaceVehicleData_t* m_RaceVehicleData;
} RaceVehicleDataInfo_t;