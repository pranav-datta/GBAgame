#ifndef MAIN_H
#define MAIN_H

#include "gba.h"

struct player {
	int row;
	int col;
	int rp;
	int cp;
	unsigned short color;
};

struct obstacle {
	int row;
	int col;
	int rp;
	int cp;
	int movingUp;
	int movingDown;
};

struct state {
	struct obstacle obstacles[6];
};

#endif
