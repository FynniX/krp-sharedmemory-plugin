#pragma once

typedef struct {
	int enable;
	int rate;
} Configuration_t;

Configuration_t getConfiguration();
