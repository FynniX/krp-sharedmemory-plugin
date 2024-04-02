#include "KRPSMP.h"

extern "C" KRPSMP_API char *GetModID() { return (char *)"krp"; }
extern "C" KRPSMP_API int GetModDataVersion() { return 6; }
extern "C" KRPSMP_API int GetInterfaceVersion() { return 9; }

/* called when software is started */
extern "C" KRPSMP_API int Startup(char *_szSavePath)
{
    configuration = new Configuration(_szSavePath);
    Configuration_t config = configuration->GetConfiguration();
    logger = new Logger(_szSavePath, true);

    logger->Log("Startup");
    logger->Log(configuration->GetPrintableConfiguration());

    // Start mappings
    PluginInfo = new MappedBuffer<PluginInfo_t>(MB_PLUGININFO_FILE_NAME);
    PluginInfo->create();
    PluginInfoView = PluginInfo->view();
    logger->Log("PluginInfo created");

    KartEventInfo = new MappedBuffer<KartEventInfo_t>(MB_KARTEVENTINFO_FILE_NAME);
    KartEventInfo->create();
    KartEventInfoView = KartEventInfo->view();
    logger->Log("KartEventInfo created");

    KartSessionInfo = new MappedBuffer<KartSessionInfo_t>(MB_KARTSESSIONINFO_FILE_NAME);
    KartSessionInfo->create();
    KartSessionInfoView = KartSessionInfo->view();
    logger->Log("KartSessionInfo created");

    KartTelemetryInfo = new MappedBuffer<KartTelemetryInfo_t>(MB_KARTTELEMETRYINFO_FILE_NAME);
    KartTelemetryInfo->create();
    KartTelemetryInfoView = KartTelemetryInfo->view();
    logger->Log("KartTelemetryInfo created");

    KartLapsInfo = new MappedBuffer<KartLapsInfo_t>(MB_KARTLAPSINFO_FILE_NAME);
    KartLapsInfo->create();
    KartLapsInfoView = KartLapsInfo->view();
    logger->Log("KartLapsInfo created");

    KartSplitsInfo = new MappedBuffer<KartSplitsInfo_t>(MB_KARTSPLITSINFO_FILE_NAME);
    KartSplitsInfo->create();
    KartSplitsInfoView = KartSplitsInfo->view();
    logger->Log("KartSplitsInfo created");

    TrackSegmentsInfo = new MappedBuffer<TrackSegmentsInfo_t>(MB_TRACKSEGMENTSINFO_FILE_NAME);
    TrackSegmentsInfo->create();
    TrackSegmentsInfoView = TrackSegmentsInfo->view();
    logger->Log("TrackSegmentsInfo created");

    RaceEventInfo = new MappedBuffer<RaceEventInfo_t>(MB_RACEEVENTINFO_FILE_NAME);
    RaceEventInfo->create();
    RaceEventInfoView = RaceEventInfo->view();
    logger->Log("RaceEventInfo created");

    RaceEntriesInfo = new MappedBuffer<RaceEntriesInfo_t>(MB_RACEENTRIESINFO_FILE_NAME);
    RaceEntriesInfo->create();
    RaceEntriesInfoView = RaceEntriesInfo->view();
    logger->Log("RaceEntriesInfo created");

    RaceSessionInfo = new MappedBuffer<RaceSessionInfo_t>(MB_RACESESSIONINFO_FILE_NAME);
    RaceSessionInfo->create();
    RaceSessionInfoView = RaceSessionInfo->view();
    logger->Log("RaceSessionInfo created");

    RaceLapsInfo = new MappedBuffer<RaceLapsInfo_t>(MB_RACELAPSINFO_FILE_NAME);
    RaceLapsInfo->create();
    RaceLapsInfoView = RaceLapsInfo->view();
    logger->Log("RaceLapsInfo created");

    RaceSplitsInfo = new MappedBuffer<RaceSplitsInfo_t>(MB_RACESPLITSINFO_FILE_NAME);
    RaceSplitsInfo->create();
    RaceSplitsInfoView = RaceSplitsInfo->view();
    logger->Log("RaceSplitsInfo created");

    RaceSpeedsInfo = new MappedBuffer<RaceSpeedsInfo_t>(MB_RACESPEEDSINFO_FILE_NAME);
    RaceSpeedsInfo->create();
    RaceSpeedsInfoView = RaceSpeedsInfo->view();
    logger->Log("RaceSpeedsInfo created");

    RaceCommunicationsInfo = new MappedBuffer<RaceCommunicationsInfo_t>(MB_RACECOMMUNICATIONSINFO_FILE_NAME);
    RaceCommunicationsInfo->create();
    RaceCommunicationsInfoView = RaceCommunicationsInfo->view();
    logger->Log("RaceCommunicationsInfo created");

    RaceClassificationInfo = new MappedBuffer<RaceClassificationInfo_t>(MB_RACECLASSIFICATIONINFO_FILE_NAME);
    RaceClassificationInfo->create();
    RaceClassificationInfoView = RaceClassificationInfo->view();
    logger->Log("RaceClassificationInfo created");

    RaceTrackPositionsInfo = new MappedBuffer<RaceTrackPositionsInfo_t>(MB_RACETRACKPOSITIONSINFO_FILE_NAME);
    RaceTrackPositionsInfo->create();
    RaceTrackPositionsInfoView = RaceTrackPositionsInfo->view();
    logger->Log("RaceTrackPositionsInfo created");

    RaceVehiclesDataInfo = new MappedBuffer<RaceVehiclesDataInfo_t>(MB_RACEVEHICLESDATAINFO_FILE_NAME);
    RaceVehiclesDataInfo->create();
    RaceVehiclesDataInfoView = RaceVehiclesDataInfo->view();
    logger->Log("RaceVehiclesDataInfo created");

    CamerasInfo = new MappedBuffer<CamerasInfo_t>(MB_CAMERASINFO_FILE_NAME);
    CamerasInfo->create();
    CamerasInfoView = CamerasInfo->view();
    logger->Log("CamerasInfo created");

    CamerasControlInfo = new MappedBuffer<CamerasControlInfo_t>(MB_CAMERASCONTROLINFO_FILE_NAME);
    CamerasControlInfo->create();
    CamerasControlInfoView = CamerasControlInfo->view();
    logger->Log("CamerasControlInfo created");

    // Set plugin info values
    PluginInfoView->m_PluginRate = config.rate;
    PluginInfoView->m_PluginVersion = KRPSMP_VERSION;
    PluginInfo->write();
    logger->Log("PluginInfo updated");

    /*
    return value is requested rate
    0 = 100hz; 1 = 50hz; 2 = 20hz; 3 = 10hz; -1 = disable
    */
    return config.enabled ? config.rate : -1;
}

/* called when software is closed */
extern "C" KRPSMP_API void Shutdown()
{
    logger->Log("Shutdown");

    // Set plugin info values
    PluginInfoView->m_GameState = GameState_t::CLOSED;
    PluginInfo->write();

    // Close mappings
    PluginInfo->close();
    logger->Log("PluginInfo closed");

    KartEventInfo->close();
    logger->Log("KartEventInfo closed");
    KartSessionInfo->close();
    logger->Log("KartSessionInfo closed");
    KartTelemetryInfo->close();
    logger->Log("KartTelemetryInfo closed");
    KartLapsInfo->close();
    logger->Log("KartLapsInfo closed");
    KartSplitsInfo->close();
    logger->Log("KartSplitsInfo closed");

    TrackSegmentsInfo->close();
    logger->Log("TrackSegmentsInfo closed");

    RaceEventInfo->close();
    logger->Log("RaceEventInfo closed");
    RaceEntriesInfo->close();
    logger->Log("RaceEntriesInfo closed");
    RaceSessionInfo->close();
    logger->Log("RaceSessionInfo closed");
    RaceLapsInfo->close();
    logger->Log("RaceLapsInfo closed");
    RaceSplitsInfo->close();
    logger->Log("RaceSplitsInfo closed");
    RaceSpeedsInfo->close();
    logger->Log("RaceSpeedsInfo closed");
    RaceCommunicationsInfo->close();
    logger->Log("RaceCommunicationsInfo closed");
    RaceClassificationInfo->close();
    logger->Log("RaceClassificationInfo closed");
    RaceTrackPositionsInfo->close();
    logger->Log("RaceTrackPositionsInfo closed");
    RaceVehiclesDataInfo->close();
    logger->Log("RaceVehiclesDataInfo closed");

    CamerasInfo->close();
    logger->Log("CamerasInfo closed");
    CamerasControlInfo->close();
    logger->Log("CamerasControlInfo closed");

    delete logger;
}

/******************************************************************************
functions to receive data from the simulated kart
******************************************************************************/

/* called when event is initialized. */
extern "C" KRPSMP_API void EventInit(void *_pData, int _iDataSize)
{
    KartEventInfoView->m_KartEvent = *(SPluginsKartEvent_t *)_pData;
    KartEventInfo->write();
    logger->Log("KartEventInfo updated");
}

/* called when event is closed. */
extern "C" KRPSMP_API void EventDeinit()
{
    SPluginsKartEvent_t data = {0};
    KartEventInfoView->m_KartEvent = data;
    KartEventInfo->write();
    logger->Log("KartEventInfo updated");

    SPluginsKartLap_t data2 = {0};
    KartLapsInfoView->_iNumLaps = 0;
    for (int i = 0; i < LAPS_AMOUNT; i++)
        KartLapsInfoView->m_KartLaps[i] = data2;

    KartLapsInfo->write();
    logger->Log("KartLapsInfo updated");

    SPluginsKartSplit_t data3 = {0};
    KartSplitsInfoView->_iNumSplits = 0;
    for (int i = 0; i < SPLITS_AMOUNT; i++)
        KartSplitsInfoView->m_KartSplits[i] = data3;

    KartSplitsInfo->write();
    logger->Log("KartSplitsInfo updated");

    TrackSegmentsInfoView->_iNumSegments = 0;
    SPluginsTrackSegment_t data4 = {0};
    for (int i = 0; i < SEGMENTS_AMOUNT; i++)
        TrackSegmentsInfoView->m_TrackSegments[i] = data4;
    for (int i = 0; i < TRACK_POINTS; i++)
        TrackSegmentsInfoView->m_RaceData[i] = 0.0f;
    TrackSegmentsInfo->write();
    logger->Log("TrackSegmentsInfo updated");
}

/* called when kart goes to track. */
extern "C" KRPSMP_API void RunInit(void *_pData, int _iDataSize)
{
    // Set plugin info values
    PluginInfoView->m_GameState = GameState_t::RUNNING;
    PluginInfo->write();
    logger->Log("PluginInfo updated");

    KartSessionInfoView->m_KartSession = *(SPluginsKartSession_t *)_pData;
    KartSessionInfo->write();
    logger->Log("KartSessionInfo updated");
}

/* called when kart leaves the track. */
extern "C" KRPSMP_API void RunDeinit()
{
    // Set plugin info values
    PluginInfoView->m_GameState = GameState_t::MENU;
    PluginInfo->write();
    logger->Log("PluginInfo updated");

    SPluginsKartSession_t data = {0};
    KartSessionInfoView->m_KartSession = data;
    KartSessionInfo->write();
    logger->Log("KartSessionInfo updated");

    KartTelemetryInfoView->_fTime = 0.0f;
    KartTelemetryInfoView->_fPos = 0.0f;
    SPluginsKartData_t data2 = {0};
    KartTelemetryInfoView->m_KartData = data2;
    KartTelemetryInfo->write();
    logger->Log("KartTelemetryInfo updated");
}

/* called when simulation is started / resumed. */
extern "C" KRPSMP_API void RunStart()
{
    // Set plugin info values
    PluginInfoView->m_GameState = GameState_t::RUNNING;
    PluginInfo->write();
    logger->Log("PluginInfo updated");
}

/* called when simulation is paused. */
extern "C" KRPSMP_API void RunStop()
{
    // Set plugin info values
    PluginInfoView->m_GameState = GameState_t::PAUSED;
    PluginInfo->write();
    logger->Log("PluginInfo updated");
}

/* called when a new lap is recorded. */
extern "C" KRPSMP_API void RunLap(void *_pData, int _iDataSize)
{
    KartLapsInfoView->_iNumLaps++;
    if (KartLapsInfoView->_iNumLaps > LAPS_AMOUNT)
        KartLapsInfoView->_iNumLaps = LAPS_AMOUNT;

    for (int i = 1; i < KartLapsInfoView->_iNumLaps; i++)
    {
        KartLapsInfoView->m_KartLaps[i - 1] = KartLapsInfoView->m_KartLaps[i];
        if (i == KartLapsInfoView->_iNumLaps - 1)
            KartLapsInfoView->m_KartLaps[i] = *(SPluginsKartLap_t *)_pData;
    }

    KartLapsInfo->write();
    logger->Log("KartLapsInfo updated");
}

/* called when a split is crossed. */
extern "C" KRPSMP_API void RunSplit(void *_pData, int _iDataSize)
{
    KartSplitsInfoView->_iNumSplits++;
    if (KartSplitsInfoView->_iNumSplits > SPLITS_AMOUNT)
        KartSplitsInfoView->_iNumSplits = SPLITS_AMOUNT;

    for (int i = 1; i < KartSplitsInfoView->_iNumSplits; i++)
    {
        KartSplitsInfoView->m_KartSplits[i - 1] = KartSplitsInfoView->m_KartSplits[i];
        if (i == KartSplitsInfoView->_iNumSplits - 1)
            KartSplitsInfoView->m_KartSplits[i] = *(SPluginsKartSplit_t *)_pData;
    }

    KartSplitsInfo->write();
    logger->Log("KartSplitsInfo updated");
}

/* _fTime is the ontrack time, in seconds. _fPos is the position on centerline, from 0 to 1. */
extern "C" KRPSMP_API void RunTelemetry(void *_pData, int _iDataSize, float _fTime, float _fPos)
{
    KartTelemetryInfoView->_fTime = _fTime;
    KartTelemetryInfoView->_fPos = _fPos;
    KartTelemetryInfoView->m_KartData = *(SPluginsKartData_t *)_pData;
    KartTelemetryInfo->write();
    logger->Log("KartTelemetryInfo updated");
}

/******************************************************************************
functions to draw
******************************************************************************/

/*
called when software is started.
Set _piNumSprites to the number of zero-separated filenames in _pszSpriteName.
Set _piNumFonts to the number of zero-separated filenames in _pszFontName.
The base path for the sprite and font files is the plugins folder.
This function is optional
*/
extern "C" KRPSMP_API int DrawInit(int *_piNumSprites, char **_pszSpriteName, int *_piNumFonts, char **_pszFontName)
{
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
extern "C" KRPSMP_API void Draw(int _iState, int *_piNumQuads, void **_ppQuad, int *_piNumString, void **_ppString)
{
    *_piNumQuads = 0;
    *_piNumString = 0;

    // Set plugin info values
    if (_iState != PluginInfoView->m_TrackState)
    {
        PluginInfoView->m_TrackState = (TrackState_t)_iState;
        PluginInfo->write();
        logger->Log("PluginInfo updated");
    }
}

/******************************************************************************
function to receive the track center line
******************************************************************************/

/* _pRaceData is a pointer to a float array with the longitudinal position of the start / finish line, splits and speedtrap. */
extern "C" KRPSMP_API void TrackCenterline(int _iNumSegments, SPluginsTrackSegment_t *_pasSegment, float *_pfRaceData)
{
    TrackSegmentsInfoView->_iNumSegments = _iNumSegments;
    for (int i = 0; i < TrackSegmentsInfoView->_iNumSegments; i++)
        TrackSegmentsInfoView->m_TrackSegments[i] = _pasSegment[i];
    for (int i = 0; i < TRACK_POINTS; i++)
        TrackSegmentsInfoView->m_RaceData[i] = _pfRaceData[i];
    TrackSegmentsInfo->write();
    logger->Log("TrackSegmentsInfo updated");
}

/******************************************************************************
functions to receive the race data
******************************************************************************/

/* called when event is initialized or a replay is loaded. */
extern "C" KRPSMP_API void RaceEvent(void *_pData, int _iDataSize)
{
    RaceEventInfoView->m_RaceEvent = *(SPluginsRaceEvent_t *)_pData;
    RaceEventInfo->write();
    logger->Log("RaceEventInfo updated");
}

extern "C" KRPSMP_API void RaceDeinit()
{
    SPluginsRaceEvent_t data = {0};
    RaceEventInfoView->m_RaceEvent = data;
    RaceEventInfo->write();
    logger->Log("RaceEventInfo updated");

    SPluginsRaceSession_t data2 = {0};
    RaceSessionInfoView->m_RaceSession = data2;
    RaceSessionInfo->write();
    logger->Log("RaceSessionInfo updated");
}

extern "C" KRPSMP_API void RaceAddEntry(void *_pData, int _iDataSize)
{
    SPluginsRaceAddEntry_t data = *(SPluginsRaceAddEntry_t *)_pData;

    int entryIndex = -1;
    for (int i = 0; i < RaceEntriesInfoView->_iNumEntries; i++)
    {
        if (RaceEntriesInfoView->m_RaceEntries[i].m_iRaceNum != data.m_iRaceNum)
            continue;
        entryIndex = i;
        break;
    }

    // RaceNum already exists
    if (entryIndex != -1)
        return;

    RaceEntriesInfoView->_iNumEntries++;
    if (RaceEntriesInfoView->_iNumEntries > ENTRIES_AMOUNT)
        RaceEntriesInfoView->_iNumEntries = ENTRIES_AMOUNT;

    RaceEntriesInfoView->m_RaceEntries[RaceEntriesInfoView->_iNumEntries - 1] = data;
    RaceEntriesInfo->write();
    logger->Log("RaceEntriesInfo updated");
}

extern "C" KRPSMP_API void RaceRemoveEntry(void *_pData, int _iDataSize)
{
    SPluginsRaceRemoveEntry_t data = *(SPluginsRaceRemoveEntry_t *)_pData;

    int entryIndex = -1;
    for (int i = 0; i < RaceEntriesInfoView->_iNumEntries; i++)
    {
        if (RaceEntriesInfoView->m_RaceEntries[i].m_iRaceNum != data.m_iRaceNum)
            continue;
        entryIndex = i;
        break;
    }

    // Not found
    if (entryIndex == -1)
        return;

    RaceEntriesInfoView->_iNumEntries--;
    if (RaceEntriesInfoView->_iNumEntries < 0)
        RaceEntriesInfoView->_iNumEntries = 0;

    for (int i = entryIndex; i < RaceEntriesInfoView->_iNumEntries; i++)
    {
        if (i + 1 == RaceEntriesInfoView->_iNumEntries)
        {
            SPluginsRaceAddEntry_t data = {0};
            RaceEntriesInfoView->m_RaceEntries[i] = data;
            break;
        }

        RaceEntriesInfoView->m_RaceEntries[i] = RaceEntriesInfoView->m_RaceEntries[i + 1];
    }

    RaceEntriesInfo->write();
    logger->Log("RaceEntriesInfo updated");
}

extern "C" KRPSMP_API void RaceSession(void *_pData, int _iDataSize)
{
    RaceSessionInfoView->m_RaceSession = *(SPluginsRaceSession_t *)_pData;
    RaceSessionInfo->write();
    logger->Log("RaceSessionInfo updated");
}

extern "C" KRPSMP_API void RaceSessionState(void *_pData, int _iDataSize)
{
    SPluginsRaceSessionState_t data = *(SPluginsRaceSessionState_t *)_pData;
    RaceSessionInfoView->m_RaceSession.m_iSession = data.m_iSession;
    RaceSessionInfoView->m_RaceSession.m_iSessionSeries = data.m_iSessionSeries;
    RaceSessionInfoView->m_RaceSession.m_iSessionState = data.m_iSessionState;
    RaceSessionInfoView->m_RaceSession.m_iSessionLength = data.m_iSessionLength;
    RaceSessionInfo->write();
    logger->Log("RaceSessionInfo updated");
}

extern "C" KRPSMP_API void RaceLap(void *_pData, int _iDataSize)
{
    RaceLapsInfoView->_iNumLaps++;
    if (RaceLapsInfoView->_iNumLaps > LAPS_AMOUNT)
        RaceLapsInfoView->_iNumLaps = LAPS_AMOUNT;

    for (int i = 1; i < RaceLapsInfoView->_iNumLaps; i++)
    {
        RaceLapsInfoView->m_RaceLaps[i - 1] = RaceLapsInfoView->m_RaceLaps[i];
        if (i == RaceLapsInfoView->_iNumLaps - 1)
            RaceLapsInfoView->m_RaceLaps[i] = *(SPluginsRaceLap_t *)_pData;
    }

    RaceLapsInfo->write();
    logger->Log("RaceLapsInfo updated");
}

extern "C" KRPSMP_API void RaceSplit(void *_pData, int _iDataSize)
{
    RaceSplitsInfoView->_iNumSplits++;
    if (RaceSplitsInfoView->_iNumSplits > SPLITS_AMOUNT)
        RaceSplitsInfoView->_iNumSplits = SPLITS_AMOUNT;

    for (int i = 1; i < RaceSplitsInfoView->_iNumSplits; i++)
    {
        RaceSplitsInfoView->m_RaceSplits[i - 1] = RaceSplitsInfoView->m_RaceSplits[i];
        if (i == RaceSplitsInfoView->_iNumSplits - 1)
            RaceSplitsInfoView->m_RaceSplits[i] = *(SPluginsRaceSplit_t *)_pData;
    }

    RaceSplitsInfo->write();
    logger->Log("RaceSplitsInfo updated");
}

extern "C" KRPSMP_API void RaceSpeed(void *_pData, int _iDataSize)
{
    SPluginsRaceSpeed_t data = *(SPluginsRaceSpeed_t *)_pData;

    int entryIndex = -1;
    for (int i = 0; i < RaceEntriesInfoView->_iNumEntries; i++)
    {
        if (RaceEntriesInfoView->m_RaceEntries[i].m_iRaceNum != data.m_iRaceNum)
            continue;
        entryIndex = i;
        break;
    }

    // Not found
    if (entryIndex == -1)
        return;

    RaceSpeedsInfoView->m_RaceSpeeds[entryIndex] = data;
    RaceSpeedsInfo->write();
    logger->Log("RaceSpeedsInfo updated");
}

extern "C" KRPSMP_API void RaceCommunication(void *_pData, int _iDataSize)
{
    RaceCommunicationsInfoView->_iNumCommunications++;
    if (RaceCommunicationsInfoView->_iNumCommunications > COMMUNICATIONS_AMOUNT)
        RaceCommunicationsInfoView->_iNumCommunications = COMMUNICATIONS_AMOUNT;

    for (int i = 1; i < RaceCommunicationsInfoView->_iNumCommunications; i++)
    {
        RaceCommunicationsInfoView->m_RaceCommunications[i - 1] = RaceCommunicationsInfoView->m_RaceCommunications[i];
        if (i == RaceCommunicationsInfoView->_iNumCommunications - 1)
            RaceCommunicationsInfoView->m_RaceCommunications[i] = *(SPluginsRaceCommunication_t *)_pData;
    }

    RaceCommunicationsInfo->write();
    logger->Log("RaceCommunicationsInfo updated");
}

/* The number of elements of _pArray if given by m_iNumEntries in _pData. */
extern "C" KRPSMP_API void RaceClassification(void *_pData, int _iDataSize, void *_pArray, int _iElemSize)
{
    SPluginsRaceClassification_t data = *(SPluginsRaceClassification_t *)_pData;
    SPluginsRaceClassificationEntry_t *array = (SPluginsRaceClassificationEntry_t *)_pArray;

    RaceClassificationInfoView->m_RaceClassification = data;

    for (int i = 0; i < data.m_iNumEntries; i++)
    {
        int oldGap = RaceClassificationInfoView->m_RaceClassificationEntries[i].m_iGap;
        int oldGapLaps = RaceClassificationInfoView->m_RaceClassificationEntries[i].m_iGapLaps;
        SPluginsRaceClassificationEntry_t entry = *(array + i);

        if ((oldGap > 0 && entry.m_iGap == 0) || (oldGapLaps > 0 && entry.m_iGapLaps == 0))
        {
            entry.m_iGap = oldGap;
            entry.m_iGapLaps = oldGapLaps;
        }

        RaceClassificationInfoView->m_RaceClassificationEntries[i] = entry;
    }

    RaceClassificationInfo->write();
    logger->Log("RaceClassificationInfo updated");
}

extern "C" KRPSMP_API void RaceTrackPosition(int _iNumVehicles, void *_pArray, int _iElemSize)
{
    SPluginsRaceTrackPosition_t *array = (SPluginsRaceTrackPosition_t *)_pArray;

    RaceTrackPositionsInfoView->_iNumVehicles = _iNumVehicles;
    if (RaceTrackPositionsInfoView->_iNumVehicles > ENTRIES_AMOUNT)
        RaceTrackPositionsInfoView->_iNumVehicles = ENTRIES_AMOUNT;

    for (int i = 0; i < RaceTrackPositionsInfoView->_iNumVehicles; i++)
        RaceTrackPositionsInfoView->m_RaceTrackPositions[i] = *(array + i);

    RaceTrackPositionsInfo->write();
    logger->Log("RaceTrackPositionsInfo updated");
}

extern "C" KRPSMP_API void RaceVehicleData(void *_pData, int _iDataSize)
{
    SPluginsRaceVehicleData_t data = *(SPluginsRaceVehicleData_t *)_pData;

    int entryIndex = -1;
    for (int i = 0; i < RaceEntriesInfoView->_iNumEntries; i++)
    {
        if (RaceEntriesInfoView->m_RaceEntries[i].m_iRaceNum != data.m_iRaceNum)
            continue;
        entryIndex = i;
        break;
    }

    // Not found
    if (entryIndex == -1)
        return;

    RaceVehiclesDataInfoView->m_RaceVehiclesData[entryIndex] = data;
    RaceVehiclesDataInfo->write();
    logger->Log("RaceVehiclesDataInfo updated");
}

/******************************************************************************
functions to control the replay
******************************************************************************/

/* Return 1 if _piSelect is set, from 0 to _iNumVehicles - 1 */
extern "C" KRPSMP_API int SpectateVehicles(int _iNumVehicles, void *_pVehicleData, int _iCurSelection, int *_piSelect)
{
    // Update CamerasInfo
    SPluginsSpectateVehicle_t *array = (SPluginsSpectateVehicle_t *)_pVehicleData;

    CamerasInfoView->_iNumVehicles = _iNumVehicles;
    if (CamerasInfoView->_iNumVehicles > ENTRIES_AMOUNT)
        CamerasInfoView->_iNumVehicles = ENTRIES_AMOUNT;

    for (int i = 0; i < CamerasInfoView->_iNumVehicles; i++)
        CamerasInfoView->m_Vehicles[i] = *(array + i);

    CamerasInfoView->currentVehicle = _iCurSelection;

    // Read CamerasControlInfo
    if (CamerasControlInfoView->selectedVehicle != _iCurSelection)
    {
        *_piSelect = CamerasControlInfoView->selectedVehicle;
        CamerasInfoView->currentVehicle = CamerasControlInfoView->selectedVehicle;
    }

    CamerasInfo->write();
    logger->Log("CamerasInfo updated");

    return CamerasControlInfoView->selectedVehicle != _iCurSelection ? 1 : 0;
}

/* Return 1 if _piSelect is set, from 0 to _iNumCameras - 1 */
extern "C" KRPSMP_API int SpectateCameras(int _iNumCameras, void *_pCameraData, int _iCurSelection, int *_piSelect)
{
    // Update CamerasInfo
    char *array = (char *)_pCameraData;

    CamerasInfoView->_iNumCameras = _iNumCameras;
    if (CamerasInfoView->_iNumCameras > CAMERAS_AMOUNT)
        CamerasInfoView->_iNumCameras = CAMERAS_AMOUNT;

    for (int i = 0; i < CamerasInfoView->_iNumCameras; i++)
    {
        strcpy_s(CamerasInfoView->m_CameraNames[i], STRING_LENGTH, array);
        array += strlen(array) + 1;
    }

    CamerasInfoView->currentCamera = _iCurSelection;

    // Read CamerasControlInfo
    if (CamerasControlInfoView->selectedCamera != _iCurSelection)
    {
        *_piSelect = CamerasControlInfoView->selectedCamera;
        CamerasInfoView->currentCamera = CamerasControlInfoView->selectedCamera;
    }

    CamerasInfo->write();
    logger->Log("CamerasInfo updated");

    return CamerasControlInfoView->selectedCamera != _iCurSelection ? 1 : 0;
}