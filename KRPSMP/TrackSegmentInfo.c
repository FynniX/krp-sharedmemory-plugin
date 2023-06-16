#include <stdio.h>
#include <windows.h>
#include "SharedFileIn.h"
#include "SharedFileOut.h"
#include "TrackSegmentInfo.h"

int initTrackSegmentInfo(FILE* logFile) {
	trackSegmentInfoFile = CreateFileMappingA(INVALID_HANDLE_VALUE, NULL, PAGE_READWRITE, 0, sizeof(TrackSegmentInfo_t), "Local\\KRPSMP_TrackSegmentInfo");

	if (trackSegmentInfoFile)
		trackSegmentInfoView = (RaceAddEntryInfo_t*)MapViewOfFile(trackSegmentInfoFile, FILE_MAP_ALL_ACCESS, 0, 0, sizeof(TrackSegmentInfo_t));

	if (!trackSegmentInfoFile || !trackSegmentInfoView)
		return -1;

	trackSegmentInfoView->m_id = -1;

	fprintf(logFile, "KRPSMP: TrackSegment Info initialised\n");

	return 1;
}

void deinitTrackSegmentInfo(FILE* logFile) {
	if (trackSegmentInfoFile || trackSegmentInfoView) {
		if (trackSegmentInfoView) {
			UnmapViewOfFile(trackSegmentInfoView);
			trackSegmentInfoView = NULL;
		}

		if (trackSegmentInfoFile)
		{
			CloseHandle(trackSegmentInfoFile);
			trackSegmentInfoFile = NULL;
		}

		fprintf(logFile, "KRPSMP: TrackSegment Info deinitialised\n");
	}
}

void updateTrackSegmentInfo(FILE* logFile) {
	trackSegmentInfoView->m_id = trackSegmentInfoView->m_id + 1;
	FlushViewOfFile(trackSegmentInfoView, sizeof(TrackSegmentInfo_t));

	fprintf(logFile, "KRPSMP: TrackSegment Info update\n");
}