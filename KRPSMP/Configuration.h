#pragma once

typedef struct {
	int logging;
	int enable;
	int rate;
} Configuration_t;

Configuration_t getConfiguration();
