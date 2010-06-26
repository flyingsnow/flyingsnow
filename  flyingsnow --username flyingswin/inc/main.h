//Jun,25,2010 Ivan He

#ifndef __MAIN_H
#define __MAIN_H

#include "type.h"
#include <INTRINS.H>
#include <sh79f166.h>
#include "timer.h"
#include "adc.h"


//
// Declare control port and pins. Oct 21,2009 by  He
#define MUTE_PIN_MASK		0x08	//GPIO_C.3

	

#define MUTE_AMP()
#define UnMUTE_AMP()

#define SYS_ON()		P0_4 = 1
#define POWER_ON()		P0_5 = 1

#define SYS_OFF()		P0_4 = 0
#define POWER_OFF()		P0_5 = 1
	
#define Read_ACC_Pin	P0_3

extern DWORD	DATA	gSysTick; 					//system tick counter


#endif
