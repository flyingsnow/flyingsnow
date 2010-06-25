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
	// prescaler 1/8
	T4PS0 = 1;
	//1ms preloader value 
	TL4 = 0x82;
	TH4 = 0xFF;
	TR4 = 1;			//Enable TIM4
	INSCON &= ~0x40;	//select Bank0
	IEN1 |= 0x40;		//Enable tim4 interrupt 
	EA = 1;				//enable global interrupt control bit									
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
