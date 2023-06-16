#pragma once

HANDLE trackSegmentInfoFile;
TrackSegmentInfo_t* trackSegmentInfoView;

int initTrackSegmentInfo(FILE*);
void deinitTrackSegmentInfo(FILE*);
void updateTrackSegmentInfo(FILE*);