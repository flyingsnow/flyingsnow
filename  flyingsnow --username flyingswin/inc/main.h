//Jun,25,2010 Ivan He

#ifndef __MAIN_H
#define __MAIN_H

#include "type.h"
#include <INTRINS.H>
#include <sh79f166.h>
#include "timer.h"


//
// Declare control port and pins. Oct 21,2009 by  He
#define MUTE_PIN_MASK		0x08	//GPIO_C.3

	
#define PWR_PIN_MASK
#define SYS_CTL_MASK

#define ACC_DET_MASK
#define ACC_IN_MASK	

#define MUTE_AMP()
#define UnMUTE_AMP()

#define SYS_ON()
#define POWER_ON()

#define SYS_OFF()
#define POWER_OFF()

#define Read_ACC_Pin
extern DWORD	DATA	gSysTick; 					//system tick counter


#endif
