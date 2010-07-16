#ifndef __CLOCK_H
#define __CLOCK_H

#include "main.h"

extern bit SetFlag;		//set hour or sec
extern bit ClockState;		//False-display clock ,True-set clock
extern UCHAR SetClockTimer;

void ClockMain(void);

#endif
