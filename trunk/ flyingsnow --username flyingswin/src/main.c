#include "main.h"
#include "interrupt.h"

bit Blink;

UCHAR t_halfsec = 0;
UCHAR t_min = 0;
UCHAR t_hour = 0;					//clock 

SYSStuct DATA System; 
KEY_EVENT	gKeyEvent = IN_KEY_NONE;

VOID  InitGpio(VOID)
{
	P0CR |= 0x30; //P0.4 P0.5 output  
	P3CR |= 0xF0; //P3_4.5 .6 .7 output
	P4CR |= 0x18; //p4.3 .4 output
}


VOID SysCommnKey(VOID)
{


	if(gKeyEvent != IN_KEY_NONE) {
		switch(gKeyEvent) {
		case IN_KEY_PWR_SP:
				if(System.PowerMode == POWERMODE_POWEROFF)
					System.PowerMode = POWERMODE_POWERONREQ;
				else {
					if(isMute) {
//						UnMUTE_AMP();
						isMute = 0;
					}
					else {
//						MUTE_AMP();
						isMute = 1;
					}

				}
			break;

		case IN_KEY_PWR_CP:
				if(System.PowerMode == POWERMODE_POWERON)
					System.PowerMode = POWERMODE_POWEROFFREQ;

			break;
			
		case IN_KEY_MOD_SP:
			if(System.PowerMode == POWERMODE_POWERON){
				if(System.WorkMode.Current == WORKMODE_RADIO) {
					SC7313_Driver(Channel_Aux);
					Si4730_Power_Down();
					TunerStatus = Status_Idle;
					System.WorkMode.Current = WORKMODE_AUX;
					System.DispMode = DISPLAY_AUX;
				}
				else {
					Tuner_Init(SaveBand,SaveFreq);
 					TunerStatus = Status_Idle;
					System.WorkMode.Current = WORKMODE_RADIO;
					System.DispMode = DISPLAY_RADIO;
					SC7313_Driver(Channel_Radio);
				}		
			}
			isMute = FALSE;
			break;
		}
	}


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
	AdcKeyScanInit();
 	while(1) {
		ACC_Check();
		if (System.AccState == ACC_ON) {
			gKeyEvent = AdcKeyEventGet();
			SysCommnKey();
			Power_main();
		//	ClockMain();
			if(System.PowerMode == POWERMODE_POWERON) {
				switch(System.WorkMode.Current) {
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

