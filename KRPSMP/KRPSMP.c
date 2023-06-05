#include <stdio.h>
#include <windows.h>

/*
If compiled as C++, extern "C" must be added to declaration of functions to export

X+ is right, Y+ is top and Z+ is forward.
*/

__declspec(dllexport) char* GetModID()
{
	return "krp";
}

__declspec(dllexport) int GetModDataVersion()
{
	return 6;
}

__declspec(dllexport) int GetInterfaceVersion()
{
	return 9;
}

/******************************************************************************
structures and functions to receive data from the simulated kart
******************************************************************************/

typedef struct
{
	char m_szDriverName[100];
	char m_szKartID[100];
	char m_szKartName[100];
	int m_iDriveType;												/* 0 = direct; 1 = clutch; 2 = shifter */
	int m_iNumberOfGears;
	int m_iMaxRPM;
	int m_iLimiter;
	int m_iShiftRPM;
	int m_iEngineCooling;											/* 0 = aircooled; 1 = watercooled */
	float m_fEngineOptTemperature;									/* degrees Celsius */
	float m_afEngineTemperatureAlarm[2];							/* degrees Celsius. Lower and upper limits */
	float m_fMaxFuel;												/* liters */
	char m_szCategory[100];
	char m_szDash[100];
	char m_szTrackID[100];
	char m_szTrackName[100];
	float m_fTrackLength;											/* centerline length. meters */
	int m_iType;													/* 1 = testing; 2 = race; 4 = challenge */
} SPluginsKartEvent_t;

typedef struct
{
	int m_iSession;													/* testing: always 0. Race: 1 = practice; 2 = qualify; 3 = warmup; 4 = qualify heat; 5 = second chance heat; 6 = prefinal; 7 = final. Challenge: 0 = waiting; 1 = practice; 2 = race */
	int m_iSessionSeries;
	int m_iConditions;												/* 0 = sunny; 1 = cloudy; 2 = rainy */
	float m_fAirTemperature;										/* degrees Celsius */
	float m_fTrackTemperature;										/* degrees Celsius */
	char m_szSetupFileName[100];
} SPluginsKartSession_t;

typedef struct
{
	int m_iRPM;														/* engine rpm */
	float m_fCylinderHeadTemperature;								/* degrees Celsius */
	float m_fWaterTemperature;										/* degrees Celsius */
	int m_iGear;													/* 0 = Neutral */
	float m_fFuel;													/* liters */
	float m_fSpeedometer;											/* meters/second */
	float m_fPosX, m_fPosY, m_fPosZ;									/* world position of a reference point attached to chassis ( not CG ) */
	float m_fVelocityX, m_fVelocityY, m_fVelocityZ;					/* velocity of CG in world coordinates. meters/second */
	float m_fAccelerationX, m_fAccelerationY, m_fAccelerationZ;		/* acceleration of CG local to chassis rotation, expressed in G ( 9.81 m/s2 ) and averaged over the latest 10ms */
	float m_aafRot[3][3];											/* rotation matrix of the chassis */
	float m_fYaw, m_fPitch, m_fRoll;									/* degrees, -180 to 180 */
	float m_fYawVelocity, m_fPitchVelocity, m_fRollVelocity;			/* degress / second */
	float m_fInputSteer;											/* degrees. Negative = left */
	float m_fInputThrottle;											/* 0 to 1 */
	float m_fInputBrake;											/* 0 to 1 */
	float m_fInputFrontBrakes;										/* 0 to 1 */
	float m_fInputClutch;											/* 0 to 1. 0 = Fully engaged */
	float m_afWheelSpeed[4];										/* meters/second. 0 = front-left; 1 = front-right; 2 = rear-left; 3 = rear-right */
	int m_aiWheelMaterial[4];										/* material index. 0 = not in contact */
	float m_fSteerTorque;											/* Nm */
} SPluginsKartData_t;

typedef struct
{
	int m_iLapNum;													/* lap index */
	int m_iInvalid;
	int m_iLapTime;													/* milliseconds */
	int m_iPos;														/* 1 = best lap */
} SPluginsKartLap_t;

typedef struct
{
	int m_iSplit;													/* split index */
	int m_iSplitTime;												/* milliseconds */
	int m_iBestDiff;												/* milliseconds. Difference with best lap */
} SPluginsKartSplit_t;

/******************************************************************************
structures to send via shared memory
******************************************************************************/

typedef struct {
	//0: software running; 1: on-track, simulation paused; 2: on-track, simulation running
	int m_GameStatus;
	//0 = 100hz; 1 = 50hz; 2 = 20hz; 3 = 10hz; -1 = disable
	int m_PluginRate;
	int m_PluginVersion;
} PluginInfo_t;

typedef struct {
	float _fTime;													/* _fTime is the ontrack time, in seconds */
	float _fPos;													/* _fPos is the position on centerline, from 0 to 1 */
	SPluginsKartData_t* m_KartData;
} TelemetryInfo_t;

FILE* g_hLog;

HANDLE pluginInfoFile;
PluginInfo_t* pluginInfoView;
HANDLE kartEventFile;
SPluginsKartEvent_t* kartEventView;
HANDLE kartSessionFile;
SPluginsKartSession_t* kartSessionView;
HANDLE kartLapFile;
SPluginsKartLap_t* kartLapView;
HANDLE kartSplitFile;
SPluginsKartSplit_t* kartSplitView;
HANDLE telemetryInfoFile;
TelemetryInfo_t* telemetryInfoView;

/* called when software is started */
__declspec(dllexport) int Startup(char* _szSavePath)
{
	int rate = 3;

	fopen_s(&g_hLog, "krpsmp_log.txt", "wt");
	fprintf(g_hLog, "KRPSMP: Startup\n");
	pluginInfoFile = CreateFileMappingA(INVALID_HANDLE_VALUE, NULL, PAGE_READWRITE, 0, sizeof(PluginInfo_t), "Local\\KRPSMP_PluginInfo");

	if (pluginInfoFile)
		pluginInfoView = (PluginInfo_t*) MapViewOfFile(pluginInfoFile, FILE_MAP_ALL_ACCESS, 0, 0, sizeof(PluginInfo_t));

	if (!pluginInfoFile || !pluginInfoView)
		return -1;

	pluginInfoView->m_PluginRate = rate;
	pluginInfoView->m_PluginVersion = 1;
	FlushViewOfFile(pluginInfoView, sizeof(PluginInfo_t));

	fprintf(g_hLog, "KRPSMP: Plugin Info started!\n");

	kartEventFile = CreateFileMappingA(INVALID_HANDLE_VALUE, NULL, PAGE_READWRITE, 0, sizeof(SPluginsKartEvent_t), "Local\\KRPSMP_KartEvent");

	if (kartEventFile)
		kartEventView = (SPluginsKartEvent_t*) MapViewOfFile(kartEventFile, FILE_MAP_ALL_ACCESS, 0, 0, sizeof(SPluginsKartEvent_t));

	if (!kartEventFile || !kartEventView)
		return -1;

	fprintf(g_hLog, "KRPSMP: Kart Event started!\n");

	kartSessionFile = CreateFileMappingA(INVALID_HANDLE_VALUE, NULL, PAGE_READWRITE, 0, sizeof(SPluginsKartSession_t), "Local\\KRPSMP_KartSession");

	if (kartSessionFile)
		kartSessionView = (SPluginsKartSession_t*)MapViewOfFile(kartSessionFile, FILE_MAP_ALL_ACCESS, 0, 0, sizeof(SPluginsKartSession_t));

	if (!kartSessionFile || !kartSessionView)
		return -1;

	fprintf(g_hLog, "KRPSMP: Kart Session started!\n");

	kartLapFile = CreateFileMappingA(INVALID_HANDLE_VALUE, NULL, PAGE_READWRITE, 0, sizeof(SPluginsKartLap_t), "Local\\KRPSMP_KartLap");

	if (kartLapFile)
		kartLapView = (SPluginsKartLap_t*)MapViewOfFile(kartSessionFile, FILE_MAP_ALL_ACCESS, 0, 0, sizeof(SPluginsKartLap_t));

	if (!kartLapFile || !kartLapView)
		return -1;

	fprintf(g_hLog, "KRPSMP: Kart Lap started!\n");

	kartSplitFile = CreateFileMappingA(INVALID_HANDLE_VALUE, NULL, PAGE_READWRITE, 0, sizeof(SPluginsKartSplit_t), "Local\\KRPSMP_KartSplit");

	if (kartSplitFile)
		kartSplitView = (SPluginsKartSplit_t*)MapViewOfFile(kartSplitFile, FILE_MAP_ALL_ACCESS, 0, 0, sizeof(SPluginsKartSplit_t));

	if (!kartSplitFile || !kartSplitView)
		return -1;

	fprintf(g_hLog, "KRPSMP: Kart Split started!\n");

	telemetryInfoFile = CreateFileMappingA(INVALID_HANDLE_VALUE, NULL, PAGE_READWRITE, 0, sizeof(TelemetryInfo_t), "Local\\KRPSMP_TelemetryInfo");

	if (telemetryInfoFile)
		telemetryInfoView = (TelemetryInfo_t*)MapViewOfFile(telemetryInfoFile, FILE_MAP_ALL_ACCESS, 0, 0, sizeof(TelemetryInfo_t));

	if (!telemetryInfoFile || !telemetryInfoView)
		return -1;

	fprintf(g_hLog, "KRPSMP: Telemetry Info started!\n");

	/*
	return value is requested rate
	0 = 100hz; 1 = 50hz; 2 = 20hz; 3 = 10hz; -1 = disable
	*/
	return rate;
}

/* called when software is closed */
__declspec(dllexport) void Shutdown()
{
	fprintf(g_hLog, "KRPSMP: Shutdown\n");
	
	pluginInfoView->m_GameStatus = -1;
	FlushViewOfFile(pluginInfoView, sizeof(PluginInfo_t));

	if (pluginInfoFile || pluginInfoView) {
		if (pluginInfoView) {
			UnmapViewOfFile(pluginInfoView);
			pluginInfoView = NULL;
		}

		if (pluginInfoFile)
		{
			CloseHandle(pluginInfoFile);
			pluginInfoFile = NULL;
		}

		fprintf(g_hLog, "KRPSMP: Plugin Info stopped!\n");
	}

	if (kartEventFile || kartEventView) {
		if (kartEventView) {
			UnmapViewOfFile(kartEventView);
			kartEventView = NULL;
		}

		if (kartEventFile)
		{
			CloseHandle(kartEventFile);
			kartEventFile = NULL;
		}

		fprintf(g_hLog, "KRPSMP: Kart Event stopped!\n");
	}

	if (kartSessionFile || kartEventView) {
		if (kartSessionView) {
			UnmapViewOfFile(kartSessionView);
			kartSessionView = NULL;
		}

		if (kartSessionFile)
		{
			CloseHandle(kartSessionFile);
			kartSessionFile = NULL;
		}

		fprintf(g_hLog, "KRPSMP: Kart Session stopped!\n");
	}

	if (kartLapFile || kartLapView) {
		if (kartLapView) {
			UnmapViewOfFile(kartLapView);
			kartLapView = NULL;
		}

		if (kartLapFile)
		{
			CloseHandle(kartLapFile);
			kartLapFile = NULL;
		}

		fprintf(g_hLog, "KRPSMP: Kart Lap stopped!\n");
	}

	if (kartSplitFile || kartSplitView) {
		if (kartSplitView) {
			UnmapViewOfFile(kartSplitView);
			kartSplitView = NULL;
		}

		if (kartSplitFile)
		{
			CloseHandle(kartSplitFile);
			kartSplitFile = NULL;
		}

		fprintf(g_hLog, "KRPSMP: Kart Split stopped!\n");
	}

	if (telemetryInfoFile || telemetryInfoView) {
		if (telemetryInfoView) {
			UnmapViewOfFile(telemetryInfoView);
			telemetryInfoView = NULL;
		}

		if (telemetryInfoFile)
		{
			CloseHandle(telemetryInfoFile);
			telemetryInfoFile = NULL;
		}

		fprintf(g_hLog, "KRPSMP: Telemetry Info stopped!\n");
	}

	if (g_hLog)
		fclose(g_hLog);
}

/* called when event is initialized. This function is optional */
__declspec(dllexport) void EventInit(void* _pData, int _iDataSize)
{
	fprintf(g_hLog, "KRPSMP: Kart Event Init\n");

	memcpy(kartEventView, (SPluginsKartEvent_t*)_pData, sizeof(SPluginsKartEvent_t));
	FlushViewOfFile(kartEventView, sizeof(SPluginsKartEvent_t));
}

/* called when event is closed. This function is optional */
__declspec(dllexport) void EventDeinit()
{
	fprintf(g_hLog, "KRPSMP: Kart Event Deinit\n");

	memset(kartEventView, 0, sizeof(SPluginsKartEvent_t));
	FlushViewOfFile(kartEventView, sizeof(SPluginsKartEvent_t));
}

/* called when kart goes to track. This function is optional */
__declspec(dllexport) void RunInit(void* _pData, int _iDataSize)
{
	fprintf(g_hLog, "KRPSMP: Kart Session Init\n");

	memcpy(kartSessionView, (SPluginsKartSession_t*)_pData, sizeof(SPluginsKartSession_t));
	FlushViewOfFile(kartSessionView, sizeof(SPluginsKartSession_t));
}

/* called when kart leaves the track. This function is optional */
__declspec(dllexport) void RunDeinit()
{
	pluginInfoView->m_GameStatus = 0;
	FlushViewOfFile(pluginInfoView, sizeof(PluginInfo_t));

	fprintf(g_hLog, "KRPSMP: Kart Session Deinit\n");

	memset(kartEventView, 0, sizeof(SPluginsKartSession_t));
	FlushViewOfFile(kartSessionView, sizeof(SPluginsKartSession_t));
}

/* called when simulation is started / resumed. This function is optional */
__declspec(dllexport) void RunStart()
{
}

/* called when simulation is paused. This function is optional */
__declspec(dllexport) void RunStop()
{
	pluginInfoView->m_GameStatus = 1;
	FlushViewOfFile(pluginInfoView, sizeof(PluginInfo_t));
}

/* called when a new lap is recorded. This function is optional */
__declspec(dllexport) void RunLap(void* _pData, int _iDataSize)
{
	fprintf(g_hLog, "KRPSMP: Kart Lap recorded\n");

	memcpy(kartLapView, (SPluginsKartLap_t*)_pData, sizeof(SPluginsKartLap_t));
	FlushViewOfFile(kartLapView, sizeof(SPluginsKartLap_t));
}

/* called when a split is crossed. This function is optional */
__declspec(dllexport) void RunSplit(void* _pData, int _iDataSize)
{
	fprintf(g_hLog, "KRPSMP: Kart Split crossed\n");

	memcpy(kartSplitView, (SPluginsKartSplit_t*)_pData, sizeof(SPluginsKartSplit_t));
	FlushViewOfFile(kartSplitView, sizeof(SPluginsKartSplit_t));
}

/* _fTime is the ontrack time, in seconds. _fPos is the position on centerline, from 0 to 1. This function is optional */
__declspec(dllexport) void RunTelemetry(void* _pData, int _iDataSize, float _fTime, float _fPos)
{
	pluginInfoView->m_GameStatus = 2;
	FlushViewOfFile(pluginInfoView, sizeof(PluginInfo_t));

	fprintf(g_hLog, "KRPSMP: Telemetry Info update\n");

	telemetryInfoView->_fTime = _fTime;
	telemetryInfoView->_fPos = _fPos;
	telemetryInfoView->m_KartData = (SPluginsKartData_t*)_pData;
	FlushViewOfFile(telemetryInfoView, sizeof(SPluginsKartData_t));
}

/******************************************************************************
structures and functions to receive the track center line
******************************************************************************/

typedef struct
{
	int m_iType;					/* 0 = straight; 1 = curve */
	float m_fLength;				/* meters */
	float m_fRadius;				/* curve radius in meters. < 0 for left curves; 0 for straights */
	float m_fAngle;					/* start angle in degrees. 0 = north */
	float m_afStart[2];				/* start position in meters */
	float m_fHeight;				/* start height in meters */
} SPluginsTrackSegment_t;

/*
_pRaceData is a pointer to a float array with the longitudinal position of the start / finish line, splits and speedtrap.
This function is optional
*/
__declspec(dllexport) void TrackCenterline(int _iNumSegments, SPluginsTrackSegment_t* _pasSegment, float* _pfRaceData)
{

}

/******************************************************************************
structures and functions to receive the race data
******************************************************************************/

typedef struct
{
	int m_iType;										/* -1 = replay; 1 = testing; 2 = race; 4 = challenge */
	char m_szName[100];
	char m_szTrackName[100];
	float m_fTrackLength;								/* meters */
} SPluginsRaceEvent_t;

typedef struct
{
	int m_iRaceNum;										/* unique race number */
	char m_szName[100];
	char m_szKartName[100];
	char m_szKartShortName[100];
	char m_szCategory[100];
	int m_iUnactive;									/* if set to 1, the driver left the event and the following fields are not set */
	int m_iNumberOfGears;
	int m_iMaxRPM;
} SPluginsRaceAddEntry_t;

typedef struct
{
	int m_iRaceNum;										/* race number */
} SPluginsRaceRemoveEntry_t;

typedef struct
{
	int m_iSession;										/* testing: always 0. Race: 1 = practice; 2 = qualify; 3 = warmup; 4 = qualify heat; 5 = second chance heat; 6 = prefinal; 7 = final. Challenge: 0 = waiting; 1 = practice; 2 = race */
	int m_iSessionSeries;
	int m_iGroup1, m_iGroup2;							/* 0 = A, 1 = B, 2 = C, ... Only used for Qualify Heats */
	int m_iSessionState;								/* testing / waiting: always 0. practice / qualify / warmup: 16 = in progress; 32 = completed. qualify heat / second chance heat / prefinal / final: 16 = in progress; 32 = semaphore; 64 = sighting lap; 128 = warmup lap; 256 = pre-start; 512 = race over; 1024 = completed; 2048 = rolling start */
	int m_iSessionLength;								/* milliseconds. 0 = no limit */
	int m_iSessionNumLaps;
	int m_iNumEntries;
	int m_aiEntries[50];
	int m_aiGrid[50];
	int m_iConditions;									/* 0 = sunny; 1 = cloudy; 2 = rainy */
	float m_fAirTemperature;							/* degrees Celsius */
	float m_fTrackTemperature;							/* degrees Celsius */
} SPluginsRaceSession_t;

typedef struct
{
	int m_iSession;										/* testing: always 0. Race: 1 = practice; 2 = qualify; 3 = warmup; 4 = qualify heat; 5 = second chance heat; 6 = prefinal; 7 = final. Challenge: 0 = waiting; 1 = practice; 2 = race */
	int m_iSessionSeries;
	int m_iSessionState;								/* testing / waiting: always 0. practice / qualify / warmup: 16 = in progress; 32 = completed. qualify heat / second chance heat / prefinal / final: 16 = in progress; 32 = semaphore; 64 = sighting lap; 128 = warmup lap; 256 = pre-start; 512 = race over; 1024 = completed; 2048 = rolling start */
	int m_iSessionLength;								/* milliseconds. 0 = no limit */
} SPluginsRaceSessionState_t;

typedef struct
{
	int m_iSession;										/* testing: always 0. Race: 1 = practice; 2 = qualify; 3 = warmup; 4 = qualify heat; 5 = second chance heat; 6 = prefinal; 7 = final. Challenge: 0 = waiting; 1 = practice; 2 = race */
	int m_iSessionSeries;
	int m_iRaceNum;										/* race number */
	int m_iLapNum;										/* lap index */
	int m_iInvalid;
	int m_iLapTime;										/* milliseconds */
	int m_aiSplit[2];									/* milliseconds */
	float m_fSpeed;										/* meters/second */
	int m_iBest;										/* 1 = personal best lap; 2 = overall best lap */
} SPluginsRaceLap_t;

typedef struct
{
	int m_iSession;										/* testing: always 0. Race: 1 = practice; 2 = qualify; 3 = warmup; 4 = qualify heat; 5 = second chance heat; 6 = prefinal; 7 = final. Challenge: 0 = waiting; 1 = practice; 2 = race */
	int m_iSessionSeries;
	int m_iRaceNum;										/* race number */
	int m_iLapNum;										/* lap index */
	int m_iSplit;										/* split index */
	int m_iSplitTime;									/* milliseconds */
} SPluginsRaceSplit_t;

typedef struct
{
	int m_iSession;										/* testing: always 0. Race: 1 = practice; 2 = qualify; 3 = warmup; 4 = qualify heat; 5 = second chance heat; 6 = prefinal; 7 = final. Challenge: 0 = waiting; 1 = practice; 2 = race */
	int m_iSessionSeries;
	int m_iRaceNum;										/* race number */
	int m_iLapNum;										/* lap index */
	float m_fSpeed;										/* meters/second */
} SPluginsRaceSpeed_t;

typedef struct
{
	int m_iSession;										/* testing: always 0. Race: 1 = practice; 2 = qualify; 3 = warmup; 4 = qualify heat; 5 = second chance heat; 6 = prefinal; 7 = final. Challenge: 0 = waiting; 1 = practice; 2 = race */
	int m_iSessionSeries;
	int m_iRaceNum;										/* race number */
	int m_iCommunication;								/* 1 = change state; 2 = penalty */
	int m_iState;										/* 1 = DNS; 2 = retired; 3 = DSQ */
	int m_iReason;										/* Reason for DSQ. 0 = jump start; 1 = too many offences; 3 = rolling start speeding; 4 = rolling start too slow; 5 = rolling start corridor crossing; 6 = rolling start overtaking; 7 = director */
	int m_iOffence;										/* 1 = jump start; 3 = cutting; 4 = rolling start speeding; 5 = rolling start too slow; 6 = rolling start corridor crossing; 7 = rolling start overtaking */
	int m_iLap;											/* lap index */
	int m_iType;										/* 1 = time penalty; 2 = position penalty */
	int m_iTime;										/* milliseconds. Penalty time */
} SPluginsRaceCommunication_t;

typedef struct
{
	int m_iSession;										/* testing: always 0. Race: 1 = practice; 2 = qualify; 3 = warmup; 4 = qualify heat; 5 = second chance heat; 6 = prefinal; 7 = final. Challenge: 0 = waiting; 1 = practice; 2 = race */
	int m_iSessionSeries;
	int m_iSessionState;								/* testing / waiting: always 0. practice / qualify / warmup: 16 = in progress; 32 = completed. qualify heat / second chance heat / prefinal / final: 16 = in progress; 32 = semaphore; 64 = sighting lap; 128 = warmup lap; 256 = pre-start; 512 = race over; 1024 = completed; 2048 = rolling start */
	int m_iSessionTime;									/* milliseconds. Current session time */
	int m_iNumEntries;									/* number of entries */
} SPluginsRaceClassification_t;

typedef struct
{
	int m_iRaceNum;										/* race number */
	int m_iState;										/* 1 = DNS; 2 = retired; 3 = DSQ */
	int m_iBestLap;										/* milliseconds */
	float m_fBestSpeed;									/* meters/second */
	int m_iBestLapNum;									/* best lap index */
	int m_iNumLaps;										/* number of laps */
	int m_iGap;											/* milliseconds */
	int m_iGapLaps;
	int m_iPenalty;										/* milliseconds */
	int m_iPit;											/* 0 = on track; 1 = in the pits */
} SPluginsRaceClassificationEntry_t;

typedef struct
{
	int m_iRaceNum;										/* race number */
	float m_fPosX, m_fPosY, m_fPosZ;						/* meters */
	float m_fYaw;										/* angle from north. degrees */
	float m_fTrackPos;									/* position on the centerline, from 0 to 1 */
} SPluginsRaceTrackPosition_t;

typedef struct
{
	int m_iRaceNum;										/* race number */
	int m_iActive;										/* if set to 0, the vehicle is not active and the following fields are not set */
	int m_iRPM;											/* engine RPM */
	int m_iGear;										/* 0 = Neutral */
	float m_fSpeedometer;								/* meters/second */
	float m_fSteer;										/* -1 ( left ) to 1 ( right ) */
	float m_fThrottle;									/* 0 to 1 */
	float m_fBrake;										/* 0 to 1 */
} SPluginsRaceVehicleData_t;

/* called when event is initialized or a replay is loaded. This function is optional */
__declspec(dllexport) void RaceEvent(void* _pData, int _iDataSize)
{
	SPluginsRaceEvent_t* psRaceEvent;

	psRaceEvent = (SPluginsRaceEvent_t*)_pData;
}

/* called when event is closed. This function is optional */
__declspec(dllexport) void RaceDeinit()
{
}

/* This function is optional */
__declspec(dllexport) void RaceAddEntry(void* _pData, int _iDataSize)
{
	SPluginsRaceAddEntry_t* psRaceAddEntry;

	psRaceAddEntry = (SPluginsRaceAddEntry_t*)_pData;
}

/* This function is optional */
__declspec(dllexport) void RaceRemoveEntry(void* _pData, int _iDataSize)
{
	SPluginsRaceRemoveEntry_t* psRaceRemoveEntry;

	psRaceRemoveEntry = (SPluginsRaceRemoveEntry_t*)_pData;
}

/* This function is optional */
__declspec(dllexport) void RaceSession(void* _pData, int _iDataSize)
{
	SPluginsRaceSession_t* psRaceSession;

	psRaceSession = (SPluginsRaceSession_t*)_pData;
}

/* This function is optional */
__declspec(dllexport) void RaceSessionState(void* _pData, int _iDataSize)
{
	SPluginsRaceSessionState_t* psRaceSessionState;

	psRaceSessionState = (SPluginsRaceSessionState_t*)_pData;
}

/* This function is optional */
__declspec(dllexport) void RaceLap(void* _pData, int _iDataSize)
{
	SPluginsRaceLap_t* psRaceLap;

	psRaceLap = (SPluginsRaceLap_t*)_pData;
}

/* This function is optional */
__declspec(dllexport) void RaceSplit(void* _pData, int _iDataSize)
{
	SPluginsRaceSplit_t* psRaceSplit;

	psRaceSplit = (SPluginsRaceSplit_t*)_pData;
}

/* This function is optional */
__declspec(dllexport) void RaceSpeed(void* _pData, int _iDataSize)
{
	SPluginsRaceSpeed_t* psRaceSpeed;

	psRaceSpeed = (SPluginsRaceSpeed_t*)_pData;
}

/* This function is optional */
__declspec(dllexport) void RaceCommunication(void* _pData, int _iDataSize)
{
	SPluginsRaceCommunication_t* psRaceCommunication;

	psRaceCommunication = (SPluginsRaceCommunication_t*)_pData;
}

/* The number of elements of _pArray if given by m_iNumEntries in _pData. This function is optional */
__declspec(dllexport) void RaceClassification(void* _pData, int _iDataSize, void* _pArray, int _iElemSize)
{
	SPluginsRaceClassification_t* psRaceClassification;
	SPluginsRaceClassificationEntry_t* pasRaceClassificationEntry;

	psRaceClassification = (SPluginsRaceClassification_t*)_pData;
	pasRaceClassificationEntry = (SPluginsRaceClassificationEntry_t*)_pArray;
}

/* This function is optional */
__declspec(dllexport) void RaceTrackPosition(int _iNumVehicles, void* _pArray, int _iElemSize)
{
	SPluginsRaceTrackPosition_t* pasRaceTrackPosition;

	pasRaceTrackPosition = (SPluginsRaceTrackPosition_t*)_pArray;
}

/* This function is optional */
__declspec(dllexport) void RaceVehicleData(void* _pData, int _iDataSize)
{
	SPluginsRaceVehicleData_t* psRaceVehicleData;

	psRaceVehicleData = (SPluginsRaceVehicleData_t*)_pData;
}
