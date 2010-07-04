#include "main.h"
#include "interrupt.h"

DWORD	DATA	gSysTick; 					//system tick counter

UCHAR t_sec = 0;
UCHAR t_min = 0;
UCHAR t_hour = 0;					//clock 

bit direction,ast,singlestep;
bit DispRefresh;
TIMER 	DATA sBlinkTimer;
KEY_EVENT	gKeyCode = IN_KEY_NONE;

VOID  InitGpio(VOID)
{
	P0CR |= 0x30; //P0.4 P0.5 output  
	P3CR |= 0xF0; //P3_4.5 .6 .7 output
	P4CR |= 0x18; //p4.3 .4 output
	
}



main()
{
	UCHAR Wait = 0x80;
	EA = 0;
	CLKCON = 0xC8;
	while(Wait--){
		_nop_();
	}
	CLKCON |= 0x04;
	SysTickInit();
	InitGpio();
	InitAdc();
	EA = 1;
	
	SYS_ON();
	POWER_ON();
	WaitMs(300);
	SC7313_initial(Channel_Radio);	
	AdcKeyScanInit();
	DisplayInit();
	Tuner_Init(SaveBand,SaveFreq);
	TimeOutSet(&sBlinkTimer,1000);
	UnMUTE_AMP();
	Wait = 0;
	while(1) {
		gKeyCode = AdcKeyEventGet();
		
		if(gKeyCode != IN_KEY_NONE){
			switch(gKeyCode) {
			case IN_KEY_NEXT_SP:
				direction = 1;
				ast = 0;
				singlestep = 0;
				status = Status_Seek;

				break;

			case IN_KEY_PRE_SP:
				direction = 0;
				ast = 0;
				singlestep = 0;
				status = Status_Seek;
			
				break;

			case IN_KEY_NEXT_CP:				
				direction = 1;
				ast = 0;
				singlestep = 1;
				status = Status_Single;

				break;

			case IN_KEY_NEXT_CPR:				
				status = Status_Idle;
				break;
			
			}
		}
		if(status != Status_Idle) {
			Tuner_Seek(direction, 0,0);
			DispRefresh = 1;
			
		}
		if(IsTimeOut(&sBlinkTimer) || DispRefresh == 1)	{	
			
				DisplayMain();	
				DispRefresh = 0;
			TimeOutSet(&sBlinkTimer,500);
			}	

	}
	return 0;
}


//
// MCU Wait Ms.
//
void
WaitMs(											
	ULONG ms										//Wait ms counter value
	)	
{
	TIMER	Delay;

	TimeOutSet(&Delay, ms); 
	while (!IsTimeOut(&Delay))
	{
	// 	FeedWatchDog();
	//	KeyEventGet();
						;
	}
}



//
// MCU Wait Us.
//
void
WaitUs(											
	ULONG us										//Wait us counter value
	)	
{
	while (us)
	{
		us--;
	}
}

