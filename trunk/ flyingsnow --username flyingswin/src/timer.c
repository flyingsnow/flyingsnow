#include "main.h"
//
// Set time out value of softtimer.
//
VOID
TimeOutSet(
	TIMER	*timer,
	DWORD 	timeout			//The maximum value is 0x80000000 ms (about 24 days)
	)
{
	IEN1 &= ~0x40;		//Disable tim4 interrupt 
	timer->RecTickVal = gSysTick;
	timer->TimeOutVal = gSysTick + timeout;		
	timer->IsTimeOut = FALSE;
	IEN1 |= 0x40;		//Enable tim4 interrupt 
			
}


//
// Check whether time out.
//
BOOL
IsTimeOut(
	TIMER 	*timer
	)
{
	IEN1 &= ~0x40;		//disable tim4 interrupt 
	if(!timer->IsTimeOut)
	{
		//After gSysTick and timer->TimeOutVal overflow, 
		//the software timer function can still work well
		//the next statement is equivalent to:
		//(gSysTick - timer->TimeOutVal) < 0x80000000 
		if((SDWORD)(gSysTick - timer->TimeOutVal) > 0)
		{
			timer->IsTimeOut = TRUE;
		}
	}	
	IEN1 |= 0x40;		//Enable tim4 interrupt 
	return timer->IsTimeOut;
}

