#include "main.h"
#include "interrupt.h"

bit DispRefresh;
bit Blink;

UCHAR t_halfsec = 0;
UCHAR t_min = 0;
UCHAR t_hour = 0;					//clock 

SYSStuct DATA System; 
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

	PowerInit();
//	POWER_ON();
	while(1) {

		ACC_Check();
		if (System.AccState == ACC_ON) {
			gKeyCode = AdcKeyEventGet();
			Power_main();
		//	ClockMain();
			if(System.PowerMode == POWERMODE_POWERON) {
				switch(System.FWorkMode.Current) {
				case WORKMODE_RADIO:
					TunerMain();
					break;

				case WORKMODE_AUX:
				//	AuxMain();
					break;
				}
				AudioMain();
			}
			DisplayMain();			
		}
	}
}

//
// MCU Wait Ms.
//
void
WaitMs(											
	ULONG ms										//Wait ms counter value
	)	
{
	UINT us = 500;
	while (ms--)
	{
		while(us--)
			;
		us = 500;
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

