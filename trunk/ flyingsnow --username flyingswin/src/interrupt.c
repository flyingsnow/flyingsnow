#include "main.h"

//
// System tick 1ms initilization.
//
void
SysTickInit(
	void
	)
{
//#warning	"Ivan: TIMER4 AS SYSTEM TICK!"
	gSysTick = 0xFFFFFFF0;
	EA = 0;
	INSCON |= 0x40; //select Bank1

	//setup TIM4 as 1ms time base 
	// prescaler 1/8
	T4PS0 = 1;
	//1ms preloader value 
	TL4 = 0x88;
	TH4 = 0xFE;
	TR4 = 1;			//Enable TIM4

	//setup Tim3 ,1s clock driver
	T3CON = 0x02;
	TR3 = 1;
	
	INSCON &= ~0x40;	//select Bank0
	IEN1 |= 0x50;		//Enable tim4 and Tim3 interrupt 
	EA = 1;				//enable global interrupt control bit									
}





void
Timer3(void)				interrupt 11
{
	t_sec++;		              /* Increment second counter */
	if(t_sec == 60)               /* 1 min is completed*/
	{
		t_sec = 0;                /* Clear second counter */
		t_min++;                  /* Increment minute counter */
		if(t_min == 60)     	  /* 1 hour is completed */
		{
			t_min = 0;            /* Clear minute counter */
			t_hour++;             /* Increment hour counter */
			if(t_hour == 24)      /* 1 day is completed */
			{
				t_hour = 0;       /* Clear hour counter */
			}
		}
	}           

}


//
// TIMER/COUNTER 4 interrupt service function.
//
void
Timer4(void)			interrupt 13		 				
{

 	if(!++(*((BYTE*)(&gSysTick) + 3)))    
 	{
  		if(!++(*((BYTE*)(&gSysTick) + 2)))
  		{
   			if(!++(*((BYTE*)(&gSysTick) + 1)))
   			{
     			(*((BYTE*)(&gSysTick) + 0))++;
   			}
  		}
 	}

}
