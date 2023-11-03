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
	SPluginsRaceLap_t m_RaceLaps[101];
} RaceLapInfo_t;

typedef struct {
	int m_id;    													/* message id */
	SPluginsRaceSplit_t m_RaceSplits[101];
} RaceSplitInfo_t;

typedef struct {
	int m_id;    													/* message id */
	SPluginsRaceSpeed_t m_RaceSpeeds[101];
} RaceSpeedInfo_t;

typedef struct {
	int m_id;    													/* message id */
	SPluginsRaceCommunication_t m_RaceCommunications[101];
} RaceCommunicationInfo_t;

typedef struct {
	int m_id;    													/* message id */
	SPluginsRaceClassification_t m_RaceClassification;
	SPluginsRaceClassificationEntry_t m_RaceEntries[100];
} RaceClassificationInfo_t;

typedef struct {
	int m_id;    													/* message id */
	int _iNumVehicles;
	SPluginsRaceTrackPosition_t m_RaceTrackPositions[100];
} RaceTrackPositionInfo_t;

typedef struct {
	int m_id;    													/* message id */
	SPluginsRaceVehicleData_t m_RaceVehicleDatas[101];
} RaceVehicleDataInfo_t;

typedef struct {
	int m_id;
	int isControlled;
	int selectedVehicle;
	int selectedCamera;
} CameraSet_t;

typedef struct {
	int _iNumVehicles;
	SPluginsSpectateVehicle_t m_VehicleData;
	int _iCurSelection;
} CameraVehiclesInfo_t;

typedef struct {
	int _iNumCameras;
	char m_CameraNames[50][100];
	int _iCurSelection;
} CameraCamerasInfo_t;

typedef struct {
	int m_id;
	CameraVehiclesInfo_t m_VehiclesInfo;
	CameraCamerasInfo_t m_CamerasInfo;
} CameraInfo_t;