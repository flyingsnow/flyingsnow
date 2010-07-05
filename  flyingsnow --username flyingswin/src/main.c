#include "main.h"
#include "interrupt.h"

DWORD	DATA	gSysTick; 					//system tick counter
bit direction,ast,singlestep;

UCHAR t_sec = 0;
UCHAR t_min = 0;
UCHAR t_hour = 0;					//clock 

SYSStuct System; 

bit DispRefresh;
UINT DATA sBlinkTimer;
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
//	SYS_ON();
//	POWER_ON();
//	WaitMs(300);
//	SC7313_initial(Channel_Radio);	
//	AdcKeyScanInit();
//	DisplayInit();
//	Tuner_Init(SaveBand,SaveFreq);
	sBlinkTimer = 0;
	UnMUTE_AMP();
	Wait = 0;
	while(1) {

		ACC_Check();
		if (System.AccState == ACC_ON) {
			gKeyCode = AdcKeyEventGet();
			Power_main();
			
			if(gKeyCode != IN_KEY_NONE){
				switch(gKeyCode) {
				case IN_KEY_PWR_SP:					
					if(System.PowerMode == POWERMODE_POWEROFF) 
						System.PowerMode = POWERMODE_POWERONREQ;
					break;

				case IN_KEY_PWR_CP:
					if (System.PowerMode == POWERMODE_POWERON)
						System.PowerMode = POWERMODE_POWEROFFREQ;
					break;
					
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

				case IN_KEY_AST_CP:
					direction = 1;
					ast = 1;
					singlestep = 0;
					status = Status_AST;
					break;

				case IN_KEY_P1_SP:
					Tuner_TunetoPreset(PRESET1);
					break;
					
				}

		        DispBuff[6] = Num[(gKeyCode/100)];
		        DispBuff[7] = Num[((gKeyCode/10)%10)];
		        DispBuff[8] = Num[(gKeyCode%10)];
				DisplayMain();
				sBlinkTimer = 200;

			}
			
			if(status != Status_Idle) {
				Tuner_Seek(direction, ast,singlestep);
				DispRefresh = 1;
				
			}
			if(sBlinkTimer == 0 || DispRefresh == 1) {
					dispFrequency(SaveBand,SaveFreq);
					DisplayMain();
					DispRefresh = 0;
					sBlinkTimer = 100;
				}
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
	UINT us = 1000;
	while (ms--)
	{
		while(us--)
			;
		us = 1000;
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

