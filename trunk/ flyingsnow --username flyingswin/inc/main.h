//Jun,25,2010 Ivan He

#ifndef __MAIN_H
#define __MAIN_H

#include "type.h"
#include <INTRINS.H>
#include <sh79f166.h>
//#include "timer.h"
#include "tuner.h"
#include "display.h"
#include "adc_key.h"
#include "si4730.h"
#include "audio.h"
#include "power.h"


typedef struct workmodestc { 
	WORKMODE Current; 
	WORKMODE Next; 
	WORKMODE Last; 
	WORKMODE Save; 
} WorkmodeStruct; 

typedef struct sysstuct { 
	POWER_MODE	PowerMode; 
	ACC_STATE	AccState;
	WorkmodeStruct 	FWorkMode; 
} SYSStuct; 
 
extern SYSStuct DATA System; 
	


extern KEY_EVENT	gKeyCode;

extern UCHAR 	t_halfsec;
extern UCHAR 	t_min;
extern UCHAR 	t_hour;
extern bit 		Blink;
//
// MCU Wait Ms.
//
extern void
WaitMs(								
	ULONG	ms										//Wait ms counter value
	);	




//
// MCU Wait Us.
//
extern void
WaitUs(											
	ULONG	us										//Wait us counter value
	);	


#endif
