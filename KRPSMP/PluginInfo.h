#pragma once

HANDLE pluginInfoFile;
PluginInfo_t* pluginInfoView;

int initPluginInfo(FILE*);
void deinitPluginInfo(FILE*);
void updatePluginInfo(FILE*);